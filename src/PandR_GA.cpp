//
// Created by rkuang on 2020/10/20.
//

#include "PandR_GA.h"
#include "ChessBoard.h"
#include "Network.h"
#include "Types.h"

#include <cstdlib>
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
            for(uint64_t j=0; j < network().nodes_at_level(i).size(); j++) {
                pos.first = random_gen(N);
                pos.second = random_gen(N);
                temp_placement.push_back(pos);
            }
        }       ///按层级顺序生成节点的坐标，第一层节点、第二层节点....(以便于后面的处理)
        individual.placement = encode(temp_placement);
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
    ///电路存在连接关系的节点之间在分配位置后之间是否存在可行的路径
    bool PandRGA::path_exist() {

    }
    ///节点之间路径是否满足时钟同步，即节点与其两个或多个finin节点之间存在的路径长度是否相等
    bool PandRGA::clock_sync() {

    }
    ///判断路径是否能正确放置实现
    bool PandRGA::path_route() {

    }
    ///个体适应度值的计算
    void PandRGA::fitness_cpt(Individual & indi) {
        std::vector<Position> real_pos;
        real_pos = decode(indi.placement);

        if(containDuplicate(indi.placement)){
            indi.fitness = 1/(2*N*N);
            return;
        }
        else if (!path_exist()){
            indi.fitness = 1/(2*N*N);
            return;
        }
        else if (!clock_sync()){

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
        for(auto i = 0; i < popu.size(); i++ ){
            sumfitness += popu[i].fitness;
        }
        ///计算个体适应度占比
        for(auto i = 0; i < popu.size(); i++){
            popu[i].refitness = popu[i].fitness / sumfitness;
        }
        ///计算累计适应度，即分配个体在0~1之间的位置区间
        popu[0].cfitness = popu[0].refitness;
        for(auto i =1; i < popu.size(); i++){
            popu[i].cfitness = popu[i-1].cfitness + popu[i].refitness;
        }
        ///轮盘赌随机选择
        for(auto i = 0; i < popu.size(); i++) {
            p = rand() % (9999 + 1) / (float)(9999 + 1);  ///定义p为[0,1]之间的一个四位小数
            if (p < popu[0].cfitness) {
                New_populations[i] = popu[0];
            }
            else {
                for (auto j = 0; j < popu.size(); j++) {
                    if (popu[j].cfitness <= p && p < popu[j + 1].cfitness) {
                        New_populations[i] = popu[j + 1];
                    }
                }
            }
        }
        ///种群更新
        for(auto i = 0; i < popu.size(); i++){
            popu[i] = New_populations[i];
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

        for(auto i = 0; i < n_populaitions; i++){
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
        for(uint64_t i = 0; i < GROUP_SCALE; i++){
            p =  rand() % (9999 + 1) / (float)(9999 + 1);
            if(p < P_MUTATION){
                for(int j = 0; j < 3; j++){
                    int l = random_gen(n_nodes);    ///改变个体中3个节点的位置坐标,随机生成下标(未考虑重复)，然后用随机生成的新的值去代替旧的值
                    uint64_t x = random_gen(N);
                    uint64_t y = random_gen(N);
                    uint64_t new_pos = x * N + y;
                    populations[i].placement[l] = new_pos;
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

        for(int i = p1; i <= p2; i++){
            temp_vec[i] = populations[x1].placement[i];
            populations[x1].placement[i] = populations[x2].placement[i];
            populations[x2].placement[i] = temp_vec[i];
        }

    }
    void PandRGA::select_the_best(){
        best_indi[0] = populations[0];      ///初始化
        for(auto i = 0;i < populations.size(); i++){
            if(best_indi[0].fitness < populations[i].fitness)           ///保留个体适应度大的个体
                best_indi[0] = populations[i];
        }
        return;
    }
}
