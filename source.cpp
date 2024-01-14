#include<iostream>
#include<vector>
#include<fstream>
#include<cmath>

class Tsp {
public:
    Tsp(const std::string filepath) {
        std::fstream file(filepath, std::ios_base::in);
        auto distance = [](const Coords& city, const Coords& other) -> float {
            return static_cast<float>(std::sqrt(std::pow(city.x - other.x, 2) + std::pow(city.y - other.y, 2)));
        };

        int n_nodes;
        file >> n_nodes;

        cost_matrix = std::vector<std::vector<float>>(n_nodes, std::vector<float>(n_nodes, 0));
        memo = std::vector<std::vector<float>>(1 << n_nodes, std::vector<float>(n_nodes, 0));
        parent_matrix = std::vector<std::vector<int>>(1 << n_nodes, std::vector<int>(n_nodes, -1));

        std::vector<Coords> city(n_nodes, {0, 0});
        for (int i = 0; i < n_nodes; i++) 
             file >> city[i].x >> city[i].y;
    
        for (int i = 0; i < n_nodes; i++) {
            for (int j = 0; j < n_nodes; j++) {
                cost_matrix[i][j] = i == j ? 0 : distance(city[i], city[j]);
            }
        }
    }

    float run(int city, int mask) {
        if (mask == ((1 << cost_matrix.size()) - 1))
            return cost_matrix[city][0];

        if (memo[mask][city] != 0) 
            return memo[mask][city];

        float res = 1000000.0;
        for (int i = 0; i < cost_matrix.size(); i++) {
            if ((mask & (1 << i)) == 0) {  
                float current_res = cost_matrix[city][i] + run(i, mask | (1 << i));
                if (current_res < res) {
                    res = current_res;
                    parent_matrix[mask][city] = i;
                }
            }
        }
        return memo[mask][city] = res;
    }

    void print_route() {
        int cur_node = 0;
        int cur_mask = 1;
        do {
            std::cout << cur_node << " ";
            cur_node = parent_matrix[cur_mask][cur_node];
            cur_mask = cur_mask | (1 << cur_node);
        } while (cur_node != -1);
    }

private:
    struct Coords {
        int x, y;
    };
    std::vector<std::vector<float>> cost_matrix;
    std::vector<std::vector<float>> memo;
    std::vector<std::vector<int>> parent_matrix;
};

int main() {
    Tsp tsp("data.txt");
    std::cout << std::ceil(tsp.run(0, 1)) << std::endl;
    tsp.print_route();
	return 0;
}

