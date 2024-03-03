#include <assert.h>
#include <stdlib.h>
#include "ts_queue.hpp"
#include "item.hpp"
#include "reader.hpp"
#include "writer.hpp"
#include "producer.hpp"
#include "consumer_controller.hpp"

#define READER_QUEUE_SIZE 200
#define WORKER_QUEUE_SIZE 200
#define WRITER_QUEUE_SIZE 4000
#define CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE 20
#define CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE 80
#define CONSUMER_CONTROLLER_CHECK_PERIOD 1000000

int main(int argc, char** argv) {
	assert(argc == 4);

	int n = atoi(argv[1]);			// opcode數量
	std::string input_file_name(argv[2]);
	std::string output_file_name(argv[3]);

	// TODO: implements main function
	// Create queues
	TSQueue<Item*> inputQueue(READER_QUEUE_SIZE);
    TSQueue<Item*> workerQueue(WORKER_QUEUE_SIZE);
    TSQueue<Item*> outputQueue(WRITER_QUEUE_SIZE);

	// Create transformer
    Transformer transformer;

	// Create reader
    Reader reader(n, input_file_name, &inputQueue);


	// Create producers
	std::vector<Producer> producers;
    for (int i = 0; i < 4; i++) {
        producers.emplace_back(&inputQueue, &workerQueue, &transformer);
    }

	// Create consumer controller
    int low_threshold = (CONSUMER_CONTROLLER_LOW_THRESHOLD_PERCENTAGE * WORKER_QUEUE_SIZE) / 100;
    int high_threshold = (CONSUMER_CONTROLLER_HIGH_THRESHOLD_PERCENTAGE *  WORKER_QUEUE_SIZE) / 100;

    ConsumerController consumerController(&workerQueue, &outputQueue, &transformer,
        CONSUMER_CONTROLLER_CHECK_PERIOD, low_threshold,
        high_threshold);


    // Create writer
    Writer writer(n, output_file_name, &outputQueue);

	// Start reader, writer, producer, and consumer controller threads
    reader.start();
    writer.start();

    for (auto& producer : producers) {
        producer.start();
    }

    consumerController.start();

    // Wait for termination
	// Join reader thread
    reader.join();

    // for (auto& producer : producers) {
    //     inputQueue.enqueue(nullptr);
    // }

    // for (auto& producer : producers) {
    //     producer.join();
    // }


    // workerQueue.enqueue(nullptr);
    // consumerController.join();


    writer.join();

	return 0;
}
