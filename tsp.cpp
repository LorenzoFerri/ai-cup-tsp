#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <time.h>
using namespace std;

struct city{
	double x;
	double y;
};

void read_input();
int distance(city a, city b);
int * random_solution(int seed,int k);
bool check_solution(int * solution, int k);
int evaluate(int * solution, int k);
void two_opt_swap(int * s , int i , int j);
void two_opt(int * s);
double acceptance_probability(double energy, double new_energy, double temperature);
void simulated_annealling(int * s);

city * cities;
int n_city;
int * distance_matrix;
clock_t start;
long custom_seed;

void read_input(int argc, const char *argv[]){
	if(argc < 2){
		cerr<<"Usage: ./tsp {tour problem} [seed]"<<endl;
		abort();
	}

	if(argc > 2){
		sscanf(argv[2],"%ld", &custom_seed);
	} else {
		custom_seed = -1;
	}

	FILE * input;
	char line[150];

	input = fopen(argv[1],"r");
	if(input ==  NULL){
		cerr<<"Error opening the file"<<endl;
		abort();
	}

	while(true){
		fgets(line,150,input);
		sscanf(line, "DIMENSION : %d", &n_city);
		sscanf(line, "DIMENSION: %d", &n_city);
		if(strcmp("NODE_COORD_SECTION\n",line) == 0) break;
	}

	cities = new city[n_city+1];
	for(int i = 1; i <= n_city; i++){
		fgets(line,150,input);
		sscanf(line, "%*d %lf %lf", &cities[i].x, &cities[i].y);
	}
	fclose(input);
}

int distance(city a, city b){
	return round(sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y)));
}

int * random_solution(){
	int * solution = new int[n_city];
	for(int i = 0; i < n_city; i++){
		solution[i] = i+1;
	}
	for (int i = n_city-1; i > 0; i--){
		int r = rand()%n_city;
		int temp = solution[r];
		solution[r] = solution[i];
		solution[i] = temp;
	}
	return solution;
}

bool check_solution(int * solution){
	for(int i = 0; i < n_city; i++ ){
		bool found = false;
		for(int j = 0; j < n_city; j++ ){
			if(solution[j] == i+1){
				found = true;
				break;
			}
		}
		if(found == false) return false;
	}
	return true;
}

int evaluate(int * solution){
	if(!check_solution(solution)){
		cerr<<"Invalid solution"<<endl;
		for(int i = 0; i < n_city; i++){
			cerr<<solution[i]<<" ";
		}
		abort();
	}
	double total = 0;
	for(int i=0; i < n_city-1; i++){
		int r = solution[i];
		int c = solution[i+1];
		total += distance_matrix[r*n_city + c];
	}
	total += distance_matrix[(solution[n_city-1])*n_city + (solution[0])];
	return total;
}

void two_opt_swap(int * s , int i , int j){
	while(i < j){
		int temp = s[i];
		s[i] = s[j];
		s[j] = temp;
		i++;
		j--;
	}
}

void two_opt(int * s){
	bool improve = true;
	double local_gain;
	double best_gain = -1;
	double new_distance;
	int first, second;
	while(improve){
		improve = false;
		for(int i = 0; i < n_city; i++){
			best_gain = 0;
			for(int j = i + 2; j < n_city; j++){
				int a = s[i];
				int b = s[i+1];
				int c = s[j];
				int d = s[(j+1) % n_city];
				if(!(distance_matrix[a * n_city + b] > distance_matrix[b * n_city + c]) && !(distance_matrix[c * n_city + d] > distance_matrix[c * n_city + a])) continue;
				if(b == c || d == a) continue;
				local_gain =
				- distance_matrix[a * n_city + b]
				- distance_matrix[c * n_city + d]
				+ distance_matrix[a * n_city + c]
				+ distance_matrix[b * n_city + d];
				if (local_gain < best_gain) {
					best_gain = local_gain;
					first = i + 1;
					second = j;
					improve = true;
				}
			}
			if(best_gain < 0){
				two_opt_swap(s, first, second);
			}
		}
	}
}

double acceptance_probability(double energy, double new_energy, double temperature) {
	if (new_energy < energy) {
		return 1.0;
	}
	return exp((energy - new_energy) / temperature);
}

void simulated_annealling(int * s){
	double duration;
	duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
	double temperature = 1000000000;
	double cooling = 0.01;
	double absTemp = 0.0000001;
	int * proposed_solution = new int[n_city];
	int * best = new int[n_city];
	memcpy(best, s, sizeof(int)*n_city);
	while(duration < 179 && temperature > absTemp) {
		memcpy(proposed_solution, s, sizeof(int)*n_city);
		int city_index_a = rand()%n_city;
		int city_index_b = rand()%n_city;
		if( city_index_b < city_index_a ){
			int temp = city_index_a;
			city_index_a = city_index_b;
			city_index_b = temp;
		}
		two_opt_swap(proposed_solution,city_index_a,city_index_b);
		two_opt(proposed_solution);
		double current_energy = evaluate(s);
		double neighbour_energy = evaluate(proposed_solution);

		double prob = acceptance_probability(current_energy, neighbour_energy, temperature);
		double rnd = ((double) rand() / (RAND_MAX));

		if(prob > rnd){
			memcpy(s, proposed_solution, sizeof(int)*n_city);
		}

		current_energy = evaluate(s);
		double best_energy = evaluate(best);
		if(current_energy < best_energy){
			memcpy(best, s, sizeof(int)*n_city);
		}

		temperature *= 1 - cooling;
		duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
	}

	memcpy(s, best, sizeof(int)*n_city);
}

int main(int argc, const char *argv[]){

	read_input(argc, argv);

	distance_matrix = new int[(n_city+1) * (n_city+1)];

	for(int i=1; i <=n_city; i++){
		for(int j=1; j <=n_city; j++){
			distance_matrix[i*n_city + j] = distance(cities[i],cities[j]);
		}
	}

	double duration;
	start = clock();

	int * s;
	long seed;
	if(custom_seed != -1){
		seed = custom_seed;
	} else {
		seed = (long)time(0);
	}

	srand(seed);

	s = random_solution();
	two_opt(s);
	simulated_annealling(s);
	int result = evaluate(s);

	for(int i = 0; i < n_city; i++){
		cout<<s[i]<<" ";
	}

	duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
	cerr<<"seed: "<< seed <<'\n';
	cerr<<"time: "<< duration <<'\n';
	cerr << "result: " << result << '\n';
	return 0;
}
