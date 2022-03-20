/*6. Sumuštinių kavinė (ADT stekas, eilė). Procesas kavinė turi 2 sumuštinių laikytuvus, iš kurių
klientas gali paimti tik viršutinį sumuštinį, o naujus sumuštinius irgi galima padėti tik ant viršaus; bet
kuriuo laiko momentu ne daugiau kaip vienas klientas užsinori sumuštinio ir paima jį iš bet kurio
laikytuvo, reguliariais laiko tarpais virtuvėje sutepama naujų sumuštinių ir visi jie sudedami į tą
laikytuvą, kuriame sumuštinių tuo momentu buvo mažiau; vakare patikrinami likę sumuštiniai ir tie,
kurie užsigulėjo per ilgai išmetami.Pradiniai duomenys tikimybė, kad bet kuriuo laiko momentu
klientas užsinorės sumuštinio, iš virtuvės vienu kartu pateikiamų sumuštinių kiekis ir periodiškumas,
kiek laiko išgulėjęs sumuštinis vakare išmetamas, vieno sumuštinio kaina, darbo dienos ilgis.Įvertinti,
kaip sumažėtų kavinės nuostoliai, jei vietoje esamų laikytuvų ji įsigytų 1 arba 2 naujus laikytuvus, iš
kurių klientas turi imti seniausią sumuštinį.*/

#include <stdio.h>
#include <stdlib.h>
#include "queue_functions.c"
#include "stack.c"

int purchaseLikelihood, newSandwichCount, newSandwichSchedule, sellByTimer, sandwichPrice, dayLength;

int getQueueSize(struct Queue * queue);

void simulateShopQueue(int counterCount, struct Queue ** counter);
void simulateShopStack(int counterCount, Stack * counter);

int main()
{
	printf("Input likelihood of purchase:\n");
	scanf("%d", &purchaseLikelihood);
	printf("Input amount of new sandwiches prepared:\n");
	scanf("%d", &newSandwichCount);
	printf("Input sandwich preparation interval:\n");
	scanf("%d", &newSandwichSchedule);
	printf("Input sell by timer:\n");
	scanf("%d", &sellByTimer);
	printf("Input price of sandwiches:\n");
	scanf("%d", &sandwichPrice);
	printf("Input length of day:\n");
	scanf("%d", &dayLength);


	srand(time(NULL));

	struct Queue * counterQueue[4]; 
	Stack counterStack[4];
	for (int i = 0; i < 4; i++)
	{
		counterQueue[i] = createQueue();
		init(&counterStack[i]);
		createStack(&counterStack[i]);
	}
	simulateShopQueue(2, counterQueue);
	simulateShopStack(2, counterStack);
	for (int i = 0; i < 4; i++)
	{
		destroyList(counterQueue[i]);
		for (int h = 0; h < countStackElements(counterStack[i]); h++)
		{
			pop(&counterStack[i]);
		}
	}
	simulateShopQueue(3, counterQueue);
	simulateShopStack(3, counterStack);
	for (int i = 0; i < 4; i++)
	{
		destroyList(counterQueue[i]);
		for (int i = 0; i < countStackElements(counterStack[i]); i++)
		{
			pop(&counterStack[i]);
		}
	}
	simulateShopQueue(4, counterQueue);
	simulateShopStack(4, counterStack);
}

void simulateShopQueue(int counterCount, struct Queue ** counter)
{
	int soldSandwichCount = 0, discardedSandwichCount = 0, newSandwichDeltaTime = newSandwichSchedule;
	int* error;
	for (int i = 0; i < dayLength; i++)
	{
		int minSandwichCount = -1;
		struct Queue* minSandwichCounter = counter[0];
		if (newSandwichDeltaTime == 0)
		{
			for (int h = 0; h < counterCount; h++)
			{
				if (minSandwichCount == -1 || (getQueueSize(counter[h]) < getQueueSize(minSandwichCounter)))
				{
					minSandwichCount = getQueueSize(counter[h]);
					minSandwichCounter = counter[h];
				}
			}

			for (int h = 0; h < newSandwichCount; h++)
			{
				enQueue(minSandwichCounter, 0, error);
			}
			newSandwichDeltaTime = newSandwichSchedule;
		}
		else
		{
			--newSandwichDeltaTime;
		}

		if (((float)rand() / RAND_MAX) * 100 <= purchaseLikelihood)
		{
				int counterIndex = ((float)rand() / RAND_MAX) * counterCount;
				if (getQueueSize(counter[counterIndex]) != 0)
				{
					deQueue(counter[counterIndex]);
					++soldSandwichCount;
				}
		}

		for (int h = 0; h < counterCount; h++)
		{
			struct QNode* node = counter[h]->front;
			while (node != NULL)
			{
				++(node->data);
				node = node->next;
			}
		}
	}

	for (int i = 0; i < counterCount; i++)
	{
		struct QNode* node = counter[i]->front;
		while (node != NULL)
		{
			if (node->data >= sellByTimer)
			{
				++discardedSandwichCount;
			}
			node = node->next;
		}
	}

	printf("Counters: %d\nSandwiches sold: %d\nSandwiches discarded: %d\nLosses: %d\n\n", counterCount, soldSandwichCount, discardedSandwichCount, discardedSandwichCount * sandwichPrice);
}

void simulateShopStack(int counterCount, Stack* counter)
{
	int soldSandwichCount = 0, discardedSandwichCount = 0, newSandwichDeltaTime = newSandwichSchedule;
	for (int i = 0; i < dayLength; i++)
	{
		int minSandwichCount = -1;
		Stack * minSandwichCounter = &counter[0];
		if (newSandwichDeltaTime == 0)
		{
			for (int h = 0; h < counterCount; h++)
			{
				if (minSandwichCount == -1 || (countStackElements(counter[h]) < countStackElements(*minSandwichCounter)))
				{
					minSandwichCount = countStackElements(counter[h]);
					minSandwichCounter = &counter[h];
				}
			}

			for (int h = 0; h < newSandwichCount; h++)
			{
				push(minSandwichCounter, 0);
			}
			newSandwichDeltaTime = newSandwichSchedule;
		}
		else
		{
			--newSandwichDeltaTime;
		}

		if (((float)rand() / RAND_MAX) * 100 <= purchaseLikelihood)
		{
			int counterIndex = ((float)rand() / RAND_MAX) * counterCount;
			if (countStackElements(counter[counterIndex]) != 0)
			{
				pop(&counter[counterIndex]);
				++soldSandwichCount;
			}
		}

		for (int h = 0; h < counterCount; h++)
		{
			List* node = counter[h].head;
			int oFlag = 0;
			for (int x = 0; x < countStackElements(counter[h]); x++)
			{
				++(node->data);
				node = node->link;
			}
		}
	}

	for (int i = 0; i < counterCount; i++)
	{
		List* node = counter[i].head;
		for (int h = 0; h < countStackElements(counter[i]); h++)
		{
			if ((node->data) >= sellByTimer)
			{
				++discardedSandwichCount;
			}
			node = node->link;
		}
	}

	printf("Counters: %d\nSandwiches sold: %d\nSandwiches discarded: %d\nLosses: %d\n\n", counterCount, soldSandwichCount, discardedSandwichCount, (discardedSandwichCount - soldSandwichCount) * sandwichPrice);
}

int getQueueSize(struct Queue * queue)
{
	int size = 0;
	struct QNode * node = queue->front;
	if (node != NULL)
	{
		while (node != NULL)
		{
			++size;
			node = node->next;
		}
	}

	return size;
}