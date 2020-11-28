#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include "mpi.h"

using namespace std;

const int MAX_ARRAY_SIZE = 1024;

// Функция, генерирующая случайное действительное число от min до max
double Random(double min, double max)
{
	srand(time(0));
	return (double)(rand()) / RAND_MAX * (max - min) + min;
}

int main(int argc, char* argv[]);

void save_data(ofstream& out, string filename, int size, double* input, double* output, int world_size, int world_rank, int& tag, MPI_Status& status);


int main(int argc, char* argv[])
{
	int init, tag = 0;

	// Initialize the MPI environment
	init = MPI_Init(NULL, NULL);

	// Get the number of processes
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);


	// Get the rank of the process
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	// Get the name of the processor
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_len;
	MPI_Get_processor_name(processor_name, &name_len);

	MPI_Status status;

	if (init != MPI_SUCCESS)
	{
		cout << "\nERROR initializing MPI. Exit.\n";
		MPI_Abort(MPI_COMM_WORLD, init);
		return 0;
	}

	cout << "Hello world from processor " << processor_name << " rank " << world_rank << " out of " << world_size << " processors\n";

	ofstream file;
	double* input = new double[MAX_ARRAY_SIZE] {0};
	double* output = new double[MAX_ARRAY_SIZE] {0};
	string filename = "time_" + to_string(world_rank) + ".txt";
	for (short int nums_proc = 1; nums_proc < world_size; nums_proc++) {
		save_data(file, filename, MAX_ARRAY_SIZE, input, output, nums_proc, world_rank, tag, status);
	}

	delete[] input;
	delete[] output;
	MPI_Finalize();
	/*MPI SECTION END*/
	return 0;
}

void save_data(ofstream& file, string filename, int size, double* input, double* output, int world_size, int world_rank, int& tag, MPI_Status& status)
{
	double time_start = 0.0, time_end = 0.0;

	file.open(filename, fstream::app);
	if (!file.is_open()) {
		cout << endl << "file didn't open" << endl;
	}
	else {
		for (int i = 1; i < size + 1; i++) {
			MPI_Barrier(MPI_COMM_WORLD);
			// to/from left
			time_start = MPI_Wtime();
			if (world_rank > 0 && world_rank <= world_size) {
				MPI_Send(&output, i, MPI_DOUBLE, world_rank - 1, tag, MPI_COMM_WORLD);
				MPI_Recv(input, i, MPI_DOUBLE, world_rank - 1, tag, MPI_COMM_WORLD, &status);
			}
			// to/from right
			if (world_rank < world_size) {
				MPI_Recv(input, i, MPI_DOUBLE, world_rank + 1, tag, MPI_COMM_WORLD, &status);
				MPI_Send(&output, i, MPI_DOUBLE, world_rank + 1, tag, MPI_COMM_WORLD);
			}
			MPI_Barrier(MPI_COMM_WORLD);
			time_end = MPI_Wtime();
			if (world_rank <= world_size) {
				file << "Processor - " << world_rank << "|" << "Block size - " <<  i << "|" << "Num processors: " << world_size + 1 << "|" << "Time - " << time_end - time_start << endl;
			}
		}
	}

	file.close();
}