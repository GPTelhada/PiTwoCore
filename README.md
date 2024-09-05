# Dual-Core Pi Calculation on ESP32-S3

## Description
This Arduino sketch demonstrates a dual-core approach to calculating Pi using the ESP32-S3 microcontroller. The sketch splits the calculation between the two cores of the ESP32-S3, improving execution time. The Leibniz series is used for the Pi approximation, with each core handling a portion of the calculation.

## How It Works
- **Core 0** handles the even-indexed terms of the Leibniz series.
- **Core 1** handles the odd-indexed terms.
- The results from both cores are synchronized using a semaphore and then combined to produce the final approximation of Pi.

## Setup
1. **Hardware Requirements**: ESP32-S3 microcontroller.
2. **Software Requirements**: Arduino IDE with ESP32 board support.
3. **Setup**: 
   - Compile and upload the sketch to your ESP32-S3.
   - Open the Serial Monitor at 115200 baud to view the results.

## Example Output

Time taken on Core 1: 1470 ms <br>
Approximated Pi value: 3.1415926535 <br>
The real value of pi: 3.1415926535 <br>
Relative Error: 0.000032 % 

## Performance Results

### Single-Core (ESP32-S3)

| Iterations | Time   | Relative Error |
|------------|--------|----------------|
| 100        | 1ms    | 0.318302%      |
| 1,000      | 3ms    | 0.031831%      |
| 10,000     | 29ms   | 0.003183%      |
| 100,000    | 289ms  | 0.000318%      |
| 1,000,000  | 2886ms | 0.000032%      |

### Dual-Core (ESP32-S3)

| Iterations | Time   | Relative Error |
|------------|--------|----------------|
| 100        | 0ms    | 0.318302%      |
| 1,000      | 1ms    | 0.031831%      |
| 10,000     | 14ms   | 0.003183%      |
| 100,000    | 147ms  | 0.000318%      |
| 1,000,000  | 1470ms | 0.000032%      |
