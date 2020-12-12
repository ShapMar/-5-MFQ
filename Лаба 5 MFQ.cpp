#include <stdlib.h> //mfq
#include <stdio.h>
#include <iostream>
#include <malloc.h>
#include <locale>
#pragma warning(disable:4996)

struct process
{
	int t, entry_time, T, dt, priority, last_tact;
};

int min(process* p, int n, int count)
{
	int min = 1000, res;
	for (int i = 0; i != n; i++)
		if ((p[i].last_tact < min) && (p[i].priority == count) && (p[i].dt))
		{
			min = p[i].last_tact;
			res = i;
		}
	return res;
};

bool check_enter_time(process* p, int k)
{
	for (int i = 0; i != k; ++i)
	{
		if (p[i].entry_time == p[k].entry_time)
		{
			printf_s("Процесс с таким временем появления уже есть, введите другое значение\n");
			return 1;
		}
	}
	return 0;
}

bool check_queue(int* p, int k)
{
	for (int i = 0; i != k; ++i)
	{
		if (p[i] >= p[k])
		{
			printf_s("У данной очереди не может быть такой приоритет\n");
			return 1;
		}
	}
	return 0;
}

int main()
{
	setlocale(LC_ALL, "russian");
	int n;
	printf("Введите количество процессов\n");
	do
		scanf_s("%d", &n);
	while (n < 1);
	process* proc = (struct process*)malloc(n * sizeof(struct process));
	for (int i = 0; i != n; i++)
	{
		printf_s("Введите время выполнения для %d процесса\n", i + 1);
		do
			scanf_s("%d", &proc[i].t);
		while (proc[i].t < 1);
		printf_s("Введите время появления для %d процесса\n", i + 1);
		do
			scanf_s("%d", &proc[i].entry_time);
		while ((proc[i].entry_time < 0) || (check_enter_time(proc, i)));
		proc[i].T = 0;
		proc[i].dt = proc[i].t;
		proc[i].priority = -1;
	}
	printf("\n");
	int queue; //сколько очередей
	printf("Введите количество очердей\n");
	do
		scanf_s("%d", &queue);
	while (queue < 1);
	int* q = (int*)malloc((queue - 1) * sizeof(int));
	for (int i = 0; i != queue - 1; i++)
	{
		printf_s("Введите приоритет %d очереди\n", i + 1);
		do
		{
			scanf_s("%d", &q[i]);
		} while ((q[i] < 1) || (check_queue(q, i)));
	}
	printf("\n");
	printf("1 - процессу был выделен квант времени в данный такт\n0 - процессу не был выделен квант времени в данный такт\n");
	int tact = 1,
		proc_enter,
		enter = 0, //сколько Q получит процесс proc_enter
		sum_dt = 0; //индикатор конца алгоритма
	for (int i = 0; i != n; i++)
		printf("%d ", i + 1);
	printf("\n");
	while (sum_dt != n)
	{
		for (int i = 0; i != n; i++) //вход процесса
			if (proc[i].entry_time == tact - 1)
			{
				proc[i].last_tact = tact;
				proc[i].priority = 0;
			}
		if (!enter)
		{
			bool breakk = 0;
			for (int j = 0; j != queue; j++) //очередь
			{
				for (int i = 0; i != n; i++)
					if ((proc[i].priority == j) && (proc[i].dt))
					{
						proc_enter = min(proc, n, j);
						if (j != queue - 1)
						{
							enter = q[j];
							breakk = 1;
							break;
						}
						else if (j = queue - 1)
						{
							enter = proc[i].dt;
							breakk = 1;
							break;
						}
					}
				if (breakk) break;
			}
		}
		if (enter)
		{
			enter--;
			proc[proc_enter].dt--;
			proc[proc_enter].last_tact = tact;
			if (!proc[proc_enter].dt)
			{
				enter = 0;
				sum_dt++;
			}
			if (!enter)
				proc[proc_enter].priority++;
			for (int i = 0; i != n; i++)
			{
				if (i == proc_enter)
				{
					proc[i].T++;
					printf("1 ");
				}
				else if ((proc[i].priority == -1) || (!proc[i].dt))
					printf("  ");
				else
				{
					proc[i].T++;
					printf("0 ");
				}

			}

		}
		printf("такт %d\n", tact);
		tact++;
	}
	double sT = 0, SM = 0, SR = 0, SP = 0;
	for (int i = 0; i != n; i++) //считаем потерянное,отношение реактивности, штрафное отношение
	{
		sT += proc[i].T;
		SM = SM + (proc[i].T - proc[i].t);
		SR += (double(proc[i].t) / proc[i].T);
		SP += (double(proc[i].T) / proc[i].t);
		printf("Процесс %d: T=%d, M=%d, R=%.2f, P=%.2f\n", i + 1, proc[i].T, (proc[i].T - proc[i].t), (double(proc[i].t) / proc[i].T), (double(proc[i].T) / proc[i].t));
	}
	printf("\n");
	printf("Tср=%.2f, Mср=%.2f, Rср=%.2f, Pср=%.2f\n", (sT / n), (SM / n), (SR / n), (SP / n));
	free(proc);
	free(q);
	system("pause");
	return 0;
}

