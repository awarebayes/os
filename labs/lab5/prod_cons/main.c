
/*
 * Producer - Consumer Problem
 * 3+ производителя и 3+ потребителя
 * Производители записывают буквы латинского алфавита по порядку: A, B, C
 * Потребители считывают эти буквы и выводят на экран
 * Комм Рязановой: надо работать с буфером как с буфером и читать по указателю, а не по индексу
 * Обязательно создать НАБОР семафоров
 * Алгоритм дейкстры. Использовать 3 семафора. Два семафора должны быть бинарными, третий - счетчик
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>

const int N = 128; // размер буфера
char *buffer_alloc; // Буфер
char *buffer_write; // Указатель на буфер, куда пишем
char *buffer_read; // Указатель на буфер, откуда читаем
key_t key = 0x1234; // ключ для семафоров
int semid; // идентификатор семафоров

const int n_prod = 3; // количество производителей
const int n_cons = 3; // количество потребителей
const int se = 0; // число пустых ячеек в буфере
const int sf = 1; // число заполненных ячеек в буфере
const int sb = 2; // бинарный семафор

struct sembuf wait_producer[2] = {
	{se, -1, 0}, // ждем, пока не освободится место в буфере
	{sb, -1, 0}  // ждем, пока не освободится бинарный семафор
};

struct sembuf signal_producer[2] = {
	{sb, 1, 0}, // освобождаем бинарный семафор
	{sf, 1, 0} // увеличиваем число заполненных ячеек в буфере
};

struct sembuf wait_consumer[2] = {
	{sf, -1, 0}, // ждем, пока не появится хотя бы одна заполненная ячейка в буфере
	{sb, -1, 0}  // ждем, пока не освободится бинарный семафор
};

struct sembuf signal_consumer[2] = {
	{sb, 1, 0}, // освобождаем бинарный семафор
	{se, 1, 0}  // увеличиваем число пустых ячеек в буфере
};

/*
 * P(S) - операция "взять", декремент. Если S <= 0, то процесс блокируется
 * Аналог - sembuf {id, -1, 0};
 * V(S) - операция "положить", инкремент. Если S был <= 0, то процесс разблокируется
 * Аналог - sembuf {id, 1, 0};
 */

_Noreturn void producer()
{
	while (1)
	{

		semop(semid, &wait_producer, 2);

		// записываем букву в буфер
		char *next_item = buffer_write + 1;
		*next_item = *buffer_write + 1;
		if (*next_item > 'Z')
			*next_item = 'A';
		if (next_item - buffer_alloc == N)
			next_item = buffer_alloc;
		buffer_write = next_item;

		semop(semid, &signal_producer, 2);
	}
}

_Noreturn void consumer()
{
	while (1)
	{
		semop(semid, &wait_consumer, 2);

		// читаем букву из буфера
		printf("%c", *buffer_read);
		fflush(stdout);
		buffer_read++;
		if (buffer_read - buffer_alloc == N)
			buffer_read = buffer_alloc;
		
		semop(semid, &signal_consumer, 2);
	}
}


int main() {
	// Создаем семафоры
	if ((semid = semget(key, 3, IPC_CREAT | 0666)) < 0) {
		perror("semget");
		exit(1);
	}

	// Инициализируем семафоры
	union semun {
		int val;
		struct semid_ds *buf;
		unsigned short *array;
	} arg;

	// Инициализируем семафор se
	arg.val = N;
	if (semctl(semid, se, SETVAL, arg) == -1)
	{
		perror("semctl");
		exit(1);
	}

	// Инициализируем семафор sf
	arg.val = 0;
	if (semctl(semid, sf, SETVAL, arg) == -1)
	{
		perror("semctl");
		exit(1);
	}

	// Инициализируем семафор sb
	arg.val = 1;
	if (semctl(semid, sb, SETVAL, arg) == -1)
	{
		perror("semctl");
		exit(1);
	}

	// Создаем буфер
	buffer_alloc = (char *)calloc(N, sizeof(char));
	if (buffer_alloc == NULL)
	{
		perror("malloc");
		exit(1);
	}
	buffer_write = buffer_alloc;
	buffer_read = buffer_alloc;
	*buffer_alloc = 'A';

	// создаем тред-производителя
	pthread_t producer_threads[n_prod];
	for (int i = 0; i < n_prod; i++)
	{
		if (pthread_create(&producer_threads[i], NULL, (void *)producer, NULL) != 0)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	// создаем тред-потребителя
	pthread_t consumer_threads[n_cons];
	for (int i = 0; i < n_cons; i++)
	{
		if (pthread_create(&consumer_threads[i], NULL, (void *)consumer, NULL) != 0)
		{
			perror("pthread_create");
			exit(1);
		}
	}

	// ждем завершения потоков
	for (int i = 0; i < n_prod; i++)
	{
		if (pthread_join(producer_threads[i], NULL) != 0)
		{
			perror("pthread_join");
			exit(1);
		}
	}
	for (int i = 0; i < n_cons; i++)
	{
		if (pthread_join(consumer_threads[i], NULL) != 0)
		{
			perror("pthread_join");
			exit(1);
		}
	}

	return 0;
}
