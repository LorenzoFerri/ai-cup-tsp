#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <sys/wait.h>

using namespace std;

int main(int argc, char * argv[]){
	if(argc < 2){
		cout<<"Usage: ./seed {tour.tsp}"<<endl;
		abort();
	}
	char last_string[50] = {}, best_string[50] = {};
	FILE * last;
	FILE * best;
	bool first = false;
	long best_seed, last_seed;
	int best_result, last_result;
	double best_time, last_time;

	strcat(best_string,argv[1]);
	strcat(best_string,".tour.best");
	strcat(last_string,argv[1]);
	strcat(last_string,".tour.last");
	char command[150] = {};

	best = fopen(best_string,"r");
	if(best == NULL){
		cout<<"No previous records of: "<<argv[1]<<" a new one will be created at the end of the first seed"<<endl;
		first = true;
	} else {
		char line[80];
		while(fgets(line,80,best)){
			sscanf(line, "seed: %ld", &best_seed);
			sscanf(line, "time: %lf", &best_time);
			sscanf(line, "result: %d", &best_result);
		}
	}
	if(best != NULL) fclose(best);

	strcat(command, "./tsp ");
	strcat(command, argv[1]);
	strcat(command, " > ");
	strcat(command, argv[1]);
	strcat(command, ".tour");
	strcat(command, " 2> ");
	strcat(command, argv[1]);
	strcat(command, ".tour.last");
	cout<<"Press CTRL + C to terminate seeding\n";
	while(true){
		int status = system(command);
		if(status == -1 || (WIFSIGNALED(status) &&
		   (WTERMSIG(status) == SIGINT || WTERMSIG(status) == SIGQUIT))) break;
		if(!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
			cerr << "Solver failed" << endl;
			return 1;
		}
		last = fopen(last_string,"r");
		char line[80];
		while(fgets(line,80,last)){
			sscanf(line, "seed: %ld", &last_seed);
			sscanf(line, "time: %lf", &last_time);
			sscanf(line, "result: %d", &last_result);
		}
		fclose(last);
		if(first == true || last_result < best_result){
			first = false;
			best_result = last_result;
			best_seed = last_seed;
			best_time = last_time;
			cout<<"────────────────────────────"<<endl
			<<"New Best:"<<endl
			<<"Cost: "<<best_result<<endl
			<<"Time: "<<best_time<<endl
			<<"Seed: "<<best_seed<<endl
			<<"Storing it to the '.best' file"<<endl;

			ofstream new_best;
			new_best.open(best_string);
			new_best<<"seed: "<<best_seed<<endl;
			new_best<<"time: "<<best_time<<endl;
			new_best<<"result: "<<best_result<<endl;
			new_best.close();
			cout<<"Done!"<<endl;
		}
	}
	return 0;
}
