#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

struct SharedData {
    pthread_mutex_t mutex;
    uint64_t counter;
};

void *first_thread(void *pdata) {
    sleep(2);
    struct SharedData *data = pdata;
    uint64_t last_printed_number = data->counter;
    printf("First thread starts\n");
    while (true) {
        if (pthread_mutex_trylock(&data->mutex) != 0) {
            continue;
        };
        if (data->counter != last_printed_number) {
            printf("First thread: step %llu\n", last_printed_number);
            last_printed_number += 1;
        };
        pthread_mutex_unlock(&data->mutex);
    }
}

void *second_thread(void *pdata) {
    struct SharedData *data = pdata;
    uint64_t last_printed_number = data->counter;
    printf("Second thread starts\n");
    while (true) {
        if (pthread_mutex_trylock(&data->mutex) != 0) {
            continue;
        };
        if (data->counter != last_printed_number) {
            printf("Second thread: step %llu\n", last_printed_number);
            last_printed_number += 1;
        };
        pthread_mutex_unlock(&data->mutex);
    }
}

int main() {
    pthread_mutex_t mutex;
    if (pthread_mutex_init(&mutex, nullptr) != 0) {
        printf("Failed to initialize mutex\n");
        return 1;
    }
    struct SharedData shared_data = { .mutex = mutex, .counter = 0 };
    pthread_t first_thread_handle;
    pthread_t second_thread_handle;
    if (pthread_create(&first_thread_handle, nullptr, first_thread,
                       &shared_data) < 0) {
        printf("Failed to create first thread\n");
        pthread_mutex_destroy(&mutex);
        return 1;
    };
    if (pthread_create(&second_thread_handle, nullptr, second_thread,
                       &shared_data) < 0) {
        printf("Failed to create second thread\n");
        pthread_cancel(first_thread_handle);
        pthread_mutex_destroy(&mutex);
        return 1;
    };
    sleep(1);
    if (pthread_mutex_lock(&shared_data.mutex) != 0) {
        printf("Failed to lock the mutex\n");
        pthread_cancel(first_thread_handle);
        pthread_cancel(second_thread_handle);
        pthread_mutex_destroy(&mutex);
        return 1;
    };
    shared_data.counter += 1;
    pthread_mutex_unlock(&shared_data.mutex);
    sleep(2);
    while (true) {
        if (pthread_mutex_trylock(&shared_data.mutex) != 0) {
            continue;
        };
        shared_data.counter += 1;
        pthread_mutex_unlock(&shared_data.mutex);
        sleep(1);
    }
    return 0;
}
