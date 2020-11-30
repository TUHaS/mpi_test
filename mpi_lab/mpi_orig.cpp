#include <iostream>
#include <fstream>
#include <ctime>
#include "mpi.h"

using namespace std;

// Функция, генерирующая случайное действительное число от min до max
double Random(double min, double max)
{
	srand(time(0));
	return (double)(rand()) / RAND_MAX * (max - min) + min;
}

int main(int argc, char* argv[]);

void Save_data(ofstream& out, const char* filename, int size, double* input, double* output, int world_size, int world_rank, int& tag, MPI_Status& status);


int main(int argc, char* argv[])
{
	const int size = 1024;
	int init, tag = 0;
	double* array = new double[size];

	for (int i = 0; i < size; i++)
		array[i] = Random(-1000.0, 1000.0);

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

	//double* rez = new double[world_size];
	double* input = new double[size] {0};
	double* output = new double[size] {0};

	if (init != MPI_SUCCESS)
	{
		cout << "\nERROR initializing MPI. Exit.\n";
		MPI_Abort(MPI_COMM_WORLD, init);
		return 0;
	}

	cout << "Hello world from processor " << processor_name << " rank " << world_rank << " out of " << world_size << " processors\n";

	ofstream file;

	switch (world_rank)
	{
	case 0:
		Save_data(file, "time0.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 1:
		Save_data(file, "time1.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 2:
		Save_data(file, "time2.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 3:
		Save_data(file, "time3.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 4:
		Save_data(file, "time4.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 5:
		Save_data(file, "time5.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 6:
		Save_data(file, "time6.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 7:
		Save_data(file, "time7.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 8:
		Save_data(file, "time8.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 9:
		Save_data(file, "time9.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 10:
		Save_data(file, "time10.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 11:
		Save_data(file, "time11.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 12:
		Save_data(file, "time12.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 13:
		Save_data(file, "time13.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 14:
		Save_data(file, "time14.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 15:
		Save_data(file, "time15.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 16:
		Save_data(file, "time16.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 17:
		Save_data(file, "time17.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 18:
		Save_data(file, "time18.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 19:
		Save_data(file, "time19.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 20:
		Save_data(file, "time20.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 21:
		Save_data(file, "time21.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 22:
		Save_data(file, "time22.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	case 23:
		Save_data(file, "time23.txt", size, input, output, world_size, world_rank, tag, status);
		break;
	default:
		cout << endl << "---- ERROR! Non processors ----" << endl;
		break;
	}

	delete[] input;
	delete[] output;

	MPI_Finalize();
	/*MPI SECTION END*/
	return 0;
}

void Save_data(ofstream& file, const char* filename, int size, double* input, double* output, int world_size, int world_rank, int& tag, MPI_Status& status)
{
	double time_start = 0.0, time_end = 0.0;

	file.open(filename);
	if (!file.is_open())
	{
		cout << endl << "file didn't open" << endl;
	}
	else
	{
		file << "\nProcessor - " << world_rank << endl;
		for (int i = 1; i < size + 1; i++)
		{
			file << "\nBlock size - " << i << endl;

			//-------------------поставить барьер-------------------
			MPI_Barrier(MPI_COMM_WORLD);
			//передача влево
			//-------------------поставить барьер-------------------
			time_start = MPI_Wtime();

			if (world_rank > 0)
			{
				MPI_Send(&output, i, MPI_DOUBLE, world_rank - 1, tag, MPI_COMM_WORLD);
			}

			//-------------------поставить барьер-------------------
			time_end = MPI_Wtime();
			file << "Left. Time send - " << time_end - time_start;

			//-------------------поставить барьер-------------------
			time_start = MPI_Wtime();
			if (world_rank < (world_size - 1))
			{
				MPI_Recv(input, i, MPI_DOUBLE, world_rank + 1, tag, MPI_COMM_WORLD, &status);
			}
			//-------------------поставить барьер-------------------
			time_end = MPI_Wtime();
			file << "; Time receive - " << time_end - time_start << endl;



			//-------------------поставить барьер-------------------
			//передача вправо
			time_start = MPI_Wtime();
			if (world_rank < (world_size - 1))
			{
				MPI_Send(&output, i, MPI_DOUBLE, world_rank + 1, tag, MPI_COMM_WORLD);
			}
			//-------------------поставить барьер-------------------
			time_end = MPI_Wtime();
			file << "Right. Time send - " << time_end - time_start;

			//-------------------поставить барьер-------------------
			time_start = MPI_Wtime();
			if (world_rank > 0)
			{
				MPI_Recv(input, i, MPI_DOUBLE, world_rank - 1, tag, MPI_COMM_WORLD, &status);
			}
			//-------------------поставить барьер-------------------
			time_end = MPI_Wtime();
			file << "; Time receive - " << time_end - time_start << endl;
		}
	}

	file.close();
}