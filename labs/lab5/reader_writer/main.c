/*
 * Reader - Writer / Читатель - Писатель
 * Монитор Хоара
 * Работает на переменой типа int которую он инкрементирует
 * 4 фукнции:
 * - StartRead - начало чтения
 * - StopRead - конец чтения
 * - StartWrite - начало записи
 * - StopWrite - конец записи
 */


#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int n_readers = 3;
const int n_writers = 3;

// Монитор Хоара
sem_t c_read; // Можно читать
sem_t c_write; // Можно писать

// это нужно вынести в семафоры
int nr = 0; // Количество читателей
int read_requests = 0; // запросы на чтение
bool wrt = false; // is busy writing

int value = 0; // значение

void start_write()
{
	// если уже есть писатель, или есть активные читатели
	if (wrt || nr > 0)
	{
		sem_wait(&c_write);
	}
	wrt = true;
}

void stop_write()
{
	wrt = false;
	if (read_requests > 0)
		sem_post(&c_read);
	else
		sem_post(&c_write);
}

void start_read()
{
	// если есть писатель
	if (wrt)
	{
		read_requests++;
		sem_wait(&c_read);
		read_requests--;
	}
	nr++;
	// можно читать, разрешаем. Будит следующего читателя. Цепная реакция
	sem_post(&c_read);
}

void stop_read()
{
	nr--;
	if (nr == 0)
		sem_post(&c_write);
}

_Noreturn void reader(void *thid)
{
	int thid_int = thid;
	while (true)
	{
		start_read();
		printf("Reader %d reads:  %d\n", thid_int, value);
		stop_read();
		sleep(1);
	}
}

_Noreturn void writer(void *thid)
{
	int thid_int = (int)thid;
	while (true)
	{
		start_write();
		value++;
		printf("Writer %d writes: %d\n", thid_int, value);
		stop_write();
		sleep(1);
	}
}

int main() {

	nr = 0;
	wrt = false;
	read_requests = 0;


	if (sem_init(&c_read, 0, 1) == -1)
	{
		perror("sem_init");
		exit(1);
	}
	if (sem_init(&c_write, 0, 1) == -1)
	{
		perror("sem_init");
		exit(1);
	}

	pthread_t reader_threads[n_readers];
	pthread_t writer_threads[n_writers];
	for (int i = 0; i < n_readers; i++)
	{
		if ((pthread_create(&reader_threads[i], NULL, reader, i)) != 0)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i < n_writers; i++)
	{
		if ((pthread_create(&writer_threads[i], NULL, writer, i)) != 0)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	for (int i = 0; i < n_readers; i++)
	{
		pthread_join(reader_threads[i], NULL);
	}

	for (int i = 0; i < n_writers; i++)
	{
		pthread_join(writer_threads[i], NULL);
	}

	return 0;
}
