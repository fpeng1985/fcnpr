//
// Created by rkuang on 2020/10/20.
//

#include "PandR_GA.h"
#include "ChessBoard.h"
#include "Network.h"

#include <random>
#include <stdlib.h>
#include <ctime>
#include <algorithm>

namespace fcnpr{

    PandRGA::PandRGA () {
        n_generation   = MAX_GEN;
        n_populaitions = GROUP_SCALE;
        N   =   DEFAULT_SIZE;
        n_nodes = node_cnt();
        populations.resize(GROUP_SCALE);
    }

    uint64_t PandRGA::random_gen(uint64_t const& m) const {
        std::random_device rd;
        std::mt19937 gen (rd());
        std::uniform_int_distribution <> dis(0,m);
        uint64_t n = dis (gen);
        return n;
    }

    uint64_t PandRGA::node_cnt() {
        uint64_t cnt;
        for(uint64_t i =0; i < network().depth(); i++){
            for(uint64_t j = 0; j < network().nodes_at_level(i).size(); j++){
                cnt++;
            }
        }
        return cnt;
    }

    Individual PandRGA::individual_gen() {
        Individual individual;
        Position pos;
        std::vector<Position> temp_placement;
        for(uint64_t i=0; i < network().depth(); i++){
            auto level_nodes = network().nodes_at_level(i);
            for(uint64_t j=0; j < level_nodes.size(); j++) {
                pos.first = random_gen(N);
                pos.second = random_gen(N);
                individual.nodes_pos.insert({level_nodes[j],pos});      ///在节点及对应的位置生成后，插入nodes_pos中保存。
                temp_placement.push_back(pos);
            }
        }       ///按层级顺序生成节点的坐标，第一层节点、第二层节点....(以便于后面的处理)
        individual.pos_encoded = encode(temp_placement);
        return individual;
    }

    std::vector<uint64_t> PandRGA::encode(std::vector<Position> const& placement) const {
        std::vector<uint64_t> vec;
        uint64_t val_encoded;
        for(auto & po : placement){
            val_encoded = po.first * N + po.second;
            vec.push_back(val_encoded);
        }
        return vec;
    }

    std::vector<Position> PandRGA::decode(std::vector<uint64_t> const& value) const {
        Position pos;
        std::vector<Position> pos_decode;
        for(auto & v : value ){
            pos.first  = v % N;
            pos.second = v / N;
            pos_decode.push_back(pos);
        }
        return pos_decode;
    }

    void PandRGA::initGroup() {
        for(auto i = 0; i< n_populaitions; i++){
            populations.push_back(individual_gen());
        }
    }
    ///节点坐标是否有重复，重复则不满足节点位置分配要求
    bool PandRGA::containDuplicate(std::vector<uint64_t> const& vec) {
        std::unordered_set<uint64_t> set(vec.size()*2);
        for(auto val : set){
            if(!set.insert(val).second)
                return true;
        }
        return false;
    }
    ///计算布局布线结果的面积，遗传算法的优化目标
    uint64_t PandRGA::area() {
        auto area = chessboard().compute_layout_area();
        return area;
    }
    ///电路存在连接关系的节点之间在分配位置后之间是否存在可行的路径(chessboard缓存所有路径后的判断，会使解空间不完备),考虑时钟方案
    bool PandRGA::path_exist(Individual & individual) {
        Route route;
        for(auto i = 1; i < network().depth(); ++i){
            auto level_node = network().nodes_at_level(i);
            for(auto it = level_node.cbegin(); it != level_node.cend(); ++it){
                auto fin_ins = network().fan_ins_of(*it);
                for(auto &finin_node : fin_ins){
                    auto pos_src = individual.nodes_pos.at(*it);
                    auto pos_tgt = individual.nodes_pos.at(finin_node);
                    route = chessboard().find_route_between(pos_src,pos_tgt).value(); ///调用chessboard函数寻找路径，可用A*算法替代
                       if(route.empty()){
                           return false;
                       }else{
                           individual.routings[i].insert({std::make_pair(*it,finin_node),route});
                       }
                }
            }
        }
        return true;
    }
    ///节点之间路径是否满足时钟同步，即节点与其两个或多个finin节点之间存在的路径长度是否相等
    bool PandRGA::clock_sync(Individual &individual) {
        for (auto i = 1; i < network().depth(); ++i) {        ///从第一层节点开始考虑  不考虑primary input节点
            auto level_node = network().nodes_at_level(i);
            for (auto it = level_node.cbegin(); it != level_node.cend(); ++it) {
                auto fin_ins = network().fan_ins_of(*it);
                for (auto &finin_node : fin_ins) {
                    auto level_distance = network().level_distance(finin_node, *it);
                    if(individual.routings[*it,finin_node].size() != 4*level_distance){  ///层级之间的路径长度是否为4或者4*level_distance判断时钟是否同步
                        return false;
                    }
                }
            }
        }
        return true;
    }
    ///判断路径是否能正确放置实现，
    bool PandRGA::path_route(std::vector<std::unordered_map<std::pair<Node, Node>, Route>> & routings) {
        for(auto i = 1; i < network().depth(); ++i){
            for(auto it = routings[i].begin(); it != routings[i].end(); ++it){
                if(!chessboard().wire_route(it->second)){
                    return false;
                }
            }
        }
        return true;
    }

    bool PandRGA::place( std::unordered_map<Node,Position> const & nodes_pos) {
        for(auto &n_p : nodes_pos ){
            if(!chessboard().place_node(n_p.second,n_p.first)){
                return false;
            }
        }
        return true;
    }

    bool PandRGA::routing(std::vector<std::unordered_map<std::pair<Node, Node>, Route>> const & routings) {
        for(auto i = 1; i < network().depth(); ++i){
            for(auto &[src_tgt,route] : routings[i]){
                if(!chessboard().wire_route(route)){
                    return false;
                }
            }
        }
        return true;
    }

    void PandRGA::clear_layout(Individual & individual) {
        for (auto &n_p : individual.nodes_pos) {
            chessboard().unplace_node(n_p.second);
        }
        for(auto i = 1; i < network().depth(); ++i) {
            for (auto &[src_tgt, route] : individual.routings[i]) {
                chessboard().wire_route(route);
            }
        }
        individual.nodes_pos.clear();
        individual.routings.clear();
    }
    ///个体适应度值的计算  若不是一个可行的解则为
    void PandRGA::fitness_cpt(Individual & indi) {
        std::vector<Position> real_pos;
        real_pos = decode(indi.pos_encoded);
        int node = 0;
        ///把节点的编号与坐标位置统一起来
        for(uint64_t i=0; i < network().depth(); ++i) {
            auto level_nodes = network().nodes_at_level(i);
            for (uint64_t j = 0; j < level_nodes.size(); ++j) {
                indi.nodes_pos.insert({level_nodes[j],real_pos[node]});
                ++node;
            }
        }

        if(containDuplicate(indi.pos_encoded)){
            indi.fitness = 1/(2*N*N);
            return;
        }
        else if (!path_exist(indi)){
            indi.fitness = 1/(2*N*N);
            return;
        }
        else if (!clock_sync(indi)){
            indi.fitness = 1/(2*N*N);
            return;
        }
        else if (!path_route(indi.routings)){
            indi.fitness = 1/(2*N*N);
            return;
        }
        else {
            if(place(indi.nodes_pos)&&routing(indi.routings)) {///在计算面积之前调用place、routing函数实现节点和线路的放置
                indi.fitness = 1 / area();
            }
            else{
                indi.fitness = 1/(2*N*N);
            }
            clear_layout(indi);    ///计算之后清除放置的节点和线路,保证只有一个chessboard类
        }
    }
    ///遗传算法的选择  轮盘赌方案 计算每个个体被选中的概率，然后生成[0,1]之间的一个随机数决定是否被选中
    void PandRGA::selection(std::vector<Individual>& popu) {
        const double a = 0.0;
        const double b = 1.0;
        double sumfitness = 0;
        double p;
        std::vector<Individual> New_populations;
        srand ((unsigned)time(NULL));
        ///计算种群适应度之和
        for(auto i = 0; i < popu.size(); ++i ){
            sumfitness += popu[i].fitness;
            popu[i].refitness = 0;
            popu[i].cfitness = 0;
        }
        ///计算个体适应度占比
        for(auto j = 0; j < popu.size(); ++j){
            popu[j].refitness = popu[j].fitness / sumfitness;
        }
        ///计算累计适应度，即分配个体在0~1之间的位置区间
        popu[0].cfitness = popu[0].refitness;
        for(auto l =1; l < popu.size(); ++l){
            popu[l].cfitness = popu[l-1].cfitness + popu[l].refitness;
        }
        ///轮盘赌随机选择
        for(auto m = 0; m < popu.size(); ++m) {
            p = rand() % (9999 + 1) / (float)(9999 + 1);  ///定义p为[0,1]之间的一个四位小数
            if (p < popu[0].cfitness) {
                New_populations[m] = popu[0];
            }
            else {
                for (auto n = 0; n < popu.size(); ++n) {
                    if (popu[n].cfitness <= p && p < popu[n + 1].cfitness) {
                        New_populations[n] = popu[n + 1];
                    }
                }
            }
        }
        ///种群更新
        for(auto k = 0; k < popu.size(); k++){
            popu[k] = New_populations[k];
        }
        return;
    }

    ///变异算子的实现  随机生成若干个随机数作为新的位置取代原来的位置（怎么考虑电路规模不同时的变异节点的数目和突变的概率？）
    void PandRGA::mutation() {
        /*  大变异算法 可以使算法接近完全随机化
        double a = 0.0;
        double b = 1.0;
        double f_max;
        double f_sum;
        double f_avg;

        for(auto i = 0; i < n_populaitions; ++i){
            f_max = std::max(f_max,populations[i].fitness);
            f_sum = f_sum + populations[i].fitness;
        }
        f_avg = f_sum / populations.size();

        double p_m;
        double k1 = 0.4;
        double k2 = 0.5;
         */
        double a = 0.0;
        double b = 1.0;
        double p;
        srand ((unsigned)time(NULL));
        for(uint64_t i = 0; i < GROUP_SCALE; ++i){
            p =  rand() % (9999 + 1) / (float)(9999 + 1);
            if(p < 0.2){
                for(int j = 0; j < 3; ++j){
                    int l = random_gen(n_nodes);    ///改变个体中3个节点的位置坐标,随机生成下标(未考虑重复)，然后用随机生成的新的值去代替旧的值
                    uint64_t x = random_gen(N);
                    uint64_t y = random_gen(N);
                    uint64_t new_pos = x * N + y;
                    populations[i].pos_encoded[l] = new_pos;
                }
            }
        }
    }
    ///交叉算子的实现 随机选择一个交叉点（下标），将0到该下标点的值（编码后的坐标值）交换
    void PandRGA::crossover() {
        std::vector<uint64_t> temp_vec(n_nodes,0);

        uint64_t x1 = random_gen(populations.size());
        uint64_t x2 = random_gen(populations.size());
        while(x1 == x2){                    ///保证交叉的两个个体是不同的个体
            x2 = random_gen(populations.size());
        }
        uint64_t point1 = random_gen(n_nodes);      ///交叉点的下标位置
        uint64_t point2 = random_gen(n_nodes);
        while(point1 == point2){                    ///保证交叉的两个个体是不同的个体
            point2 = random_gen(n_nodes);
        }
        int p1 = (point2 - point1) > 0 ? point1 : point2;
        int p2 = (point2 - point1) > 0 ? point2 : point1;

        for(int i = p1; i <= p2; ++i){
            temp_vec[i] = populations[x1].pos_encoded[i];
            populations[x1].pos_encoded[i] = populations[x2].pos_encoded[i];
            populations[x2].pos_encoded[i] = temp_vec[i];
        }

    }
    ///计算适应度后，选出适应度最大的一个个体（这个值不能为1/(2*N*N)）
    void PandRGA::select_the_best(){
        best_indi[0] = populations[0];      ///初始化
        for(auto i = 1;i < populations.size(); ++i){
            if(best_indi[0].fitness < populations[i].fitness)           ///保留个体适应度大的个体
                best_indi[0] = populations[i];
        }
        return;
    }

    ///只有当适应度函数的值为面积的倒数时输出best_indi中的值，否则输出no solution；
    bool PandRGA::run() {
        initGroup();
        for(int i = 0; i < n_generation; ++i){
            fitness_cpt(populations[i]);
            select_the_best();
            selection(populations);
            crossover();
            mutation();
        }
        if(best_indi[0].fitness  > 1/(2*N*N)){  ///通过适应度值判断是否是一个可行解
            return true;
        }else{
            return false;
        }
    }

    ///打印结果，将best_indi中保存的个体打印（如果存在 适应度值为area的倒数时）
    void PandRGA::pr_result() {
        auto nodes_position = decode(best_indi[0].pos_encoded);
        Node node = 1;      ///记录节点编号
        std::cout << "Area: " << area() << std::endl;
        std::cout << "Primary Inputs: " << std::endl;

        auto n_level0 = network().nodes_at_level(0).size();
        for(auto i = 0; i < n_level0; ++i){
            std::cout <<  node << ":" << nodes_position[i]<< std::endl;   ///节点的编号与存储下标对应
            ++node;
        }

        for(auto j = 1; j < network().depth(); ++j) {
            std::cout << "Level " << j << std::endl;
            std::cout << "Node Positions: " << std::endl;
            for(auto k = 0 ; k < network().nodes_at_level(j).size(); ++k) {
                std::cout << node << " : " << nodes_position[k] << std::endl;
                ++node;
            }
            std::cout << "Routings: " << std::endl;
            for(auto &[src_tgt, route] : best_indi[0].routings[j]) {
                std::cout << src_tgt.first << " => " << src_tgt.second << " : ";
                for(auto &pos : route) {
                    std::cout << pos << " ";
                }
                std::cout << std::endl;
            }
        }
    }
}
