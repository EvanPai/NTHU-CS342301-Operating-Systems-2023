#include <pthread.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include "consumer.hpp"
#include "ts_queue.hpp"
#include "item.hpp"
#include "transformer.hpp"

#ifndef CONSUMER_CONTROLLER
#define CONSUMER_CONTROLLER

class ConsumerController : public Thread {
public:
	// constructor
	ConsumerController(
		TSQueue<Item*>* worker_queue,
		TSQueue<Item*>* writer_queue,
		Transformer* transformer,
		int check_period,
		int low_threshold,
		int high_threshold
	);

	// destructor
	~ConsumerController();

	virtual void start();

private:
	std::vector<Consumer*> consumers;

	TSQueue<Item*>* worker_queue;
	TSQueue<Item*>* writer_queue;

	Transformer* transformer;

	// Check to scale down or scale up every check period in microseconds.
	int check_period;
	// When the number of items in the worker queue is lower than low_threshold,
	// the number of consumers scaled down by 1.
	int low_threshold;
	// When the number of items in the worker queue is higher than high_threshold,
	// the number of consumers scaled up by 1.
	int high_threshold;

	static void* process(void* arg);
};

// Implementation start

ConsumerController::ConsumerController(
	TSQueue<Item*>* worker_queue,
	TSQueue<Item*>* writer_queue,
	Transformer* transformer,
	int check_period,
	int low_threshold,
	int high_threshold
) : worker_queue(worker_queue),
	writer_queue(writer_queue),
	transformer(transformer),
	check_period(check_period),
	low_threshold(low_threshold),
	high_threshold(high_threshold) {
}

ConsumerController::~ConsumerController() {}

void ConsumerController::start() {
	// TODO: starts a ConsumerController thread
	pthread_create(&t, 0, ConsumerController::process, (void*)this);
}

void* ConsumerController::process(void* arg) {
	// TODO: implements the ConsumerController's work
	ConsumerController* controller = (ConsumerController*)arg;
	int consumerCount = 0;

    while (true) {
        usleep(controller->check_period);

        int workerQueueSize = controller->worker_queue->get_size();

        if (workerQueueSize > controller->high_threshold ) {
            // Create a new Consumer thread
            Consumer* newConsumer = new Consumer(controller->worker_queue, controller->writer_queue, controller->transformer);
            newConsumer->start();
            controller->consumers.push_back(newConsumer);

			std::cout << "Scaling up consumers from " << consumerCount << " to " << consumerCount+1 << std::endl;
			consumerCount++;
        }
        else if (workerQueueSize < controller->low_threshold && controller->consumers.size() > 1) {
            // Cancel the newest Consumer thread
			Consumer* newestConsumer = controller->consumers.back();
			newestConsumer->cancel();
			controller->consumers.pop_back();
			std::cout << "Scaling down consumers from " << consumerCount << " to " << consumerCount-1 << std::endl;
			consumerCount--;
        }

    }

    return nullptr;
}

#endif // CONSUMER_CONTROLLER_HPP
