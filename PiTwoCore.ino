#include <Arduino.h>

// ONE CORE (ESP32-S3)
// The following table shows the execution time and relative error
// for different numbers of iterations when the code runs on one core.
/*           Time    RelativeError
100       ->  1ms      0.318302%
1'000     ->  3ms      0.031831%
10'000    ->  29ms     0.003183%
100'000   ->  289ms    0.000318%
1'000'000 ->  2886ms   0.000032%
*/

// Dual CORE (ESP32-S3)
// The following table shows the execution time and relative error
// for different numbers of iterations when the code runs on both cores.
/*           Time  RelativeError
100       ->  0ms      0.318302%
1'000     ->  1ms      0.031831%
10'000    ->  14ms     0.003183%
100'000   ->  147ms    0.000318%
1'000'000 ->  1470ms   0.000032%
*/

const long numIterations = 100000; // Number of iterations for Pi approximation
double pi = 0.0;                   // Variable to store the approximated value of Pi
double pi_helper = 0.0;            // Helper variable for partial Pi calculation on the second core
double truePi = 3.1415926535;      // Known value of Pi for comparison

SemaphoreHandle_t syncSemaphore1;  // Semaphore for synchronizing the two tasks

// Function to calculate the relative error between the approximated Pi and the true value of Pi
double calculateRelativeError(double approximatedPi, double truePi) {
  return abs((approximatedPi - truePi) / truePi) * 100.0;
}

// Task function to calculate part of the Pi approximation on the second core
void CalculatePiHelper(void *parameter) {
  xSemaphoreTake(syncSemaphore1, portMAX_DELAY); // Wait for the semaphore to be given
  for (int i = 0; i < numIterations; i += 2) {  // Perform calculations on even terms
    pi_helper += 4.0 / (2 * i + 1);             // Add the value to pi_helper
  }
  xSemaphoreGive(syncSemaphore1);               // Release the semaphore
  vTaskDelete(NULL);                            // Delete this task when done
}

// Function to calculate the remaining part of Pi on the first core
void CalculatePi() {
  unsigned long startTime = millis();           // Record the start time
  for (int k = 1; k < numIterations; k += 2) {  // Perform calculations on odd terms
    pi -= 4.0 / (2 * k + 1);                    // Subtract the value from pi
  }
  xSemaphoreTake(syncSemaphore1, portMAX_DELAY); // Wait for the helper task to finish
  pi += pi_helper;                               // Add the result from the second core
  unsigned long endTime = millis();              // Record the end time
  unsigned long duration = endTime - startTime;  // Calculate the duration

  // Output the results
  Serial.print("Time taken on Core ");
  Serial.print(xPortGetCoreID());
  Serial.print(": ");
  Serial.print(duration);
  Serial.println(" ms");
  Serial.print("Approximated Pi value: ");
  Serial.println(pi, 10); // Print Pi with 10 decimal places
  Serial.println("The real value of pi:  3.1415926535");

  // Calculate and print the relative error
  double error = calculateRelativeError(pi, truePi);
  Serial.print("Relative Error: ");
  Serial.print(error, 6);
  Serial.println(" %");
}

void setup() {
  Serial.begin(115200); // Initialize serial communication at 115200 baud
  while (!Serial) {
    ; // Wait for the serial port to connect. Needed for native USB
  }

  syncSemaphore1 = xSemaphoreCreateBinary(); // Create binary semaphore for synchronization

  // Create the helper task on Core 0
  xTaskCreatePinnedToCore(
    CalculatePiHelper,   // Function to be executed
    "CalculatePiHelper", // Name of the task
    100000,              // Stack size in words
    NULL,                // Task input parameter
    1,                   // Priority of the task
    NULL,                // Task handle
    0);                  // Core to run the task on (0 or 1)

  xSemaphoreGive(syncSemaphore1); // Start the helper task by giving the semaphore

  Serial.println("\nDual-core Pi Calculation");
  CalculatePi(); // Start the main Pi calculation on the current core
}

void loop() {
  // Nothing to do here
}
