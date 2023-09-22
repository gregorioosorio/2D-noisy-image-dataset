#!/bin/bash

# Output directory
output_dir="./img/adf"

# Check if the ./img/adf directory exists; if not, create it
if [ ! -d "$output_dir" ]; then
    mkdir -p "$output_dir"
fi

# List of input images
input_images=(
    "./img/dataset/noisy_3/noisy_3_127.png"
    "./img/dataset/noisy_3/noisy_3_130.png"
    "./img/dataset/noisy_3/noisy_3_145.png"
    # Add more image paths here if needed
)

# List of parameter sets (iteration, time_step, and conductance)
param_sets=(
    "200 0.01 10"
    "200 0.01 5"
    "200 0.01 1"
    "200 0.1 1"
    "200 0.005 1"
    "200 0.0075 1"
)

# Function to execute the program if the output file does not exist
run_ADFFilter() {
    input_image="$1"
    iteration="$2"
    time_step="$3"
    conductance="$4"
    output_image="$output_dir/$(basename "${input_image%.*}")_adf_${iteration}_${time_step}_${conductance}.png"
    if [ ! -f "$output_image" ]; then
        ./build/ADFFilter "$input_image" "$output_image" "$iteration" "$time_step" "$conductance"
    else
        echo "Output image $output_image already exists. Skipping execution."
    fi
}

# Loop through the list of input images
for ((i = 0; i < ${#input_images[@]}; i++)); do
    input_image="${input_images[i]}"
    for param_set in "${param_sets[@]}"; do
        set -- $param_set
        iteration="$1"
        time_step="$2"
        conductance="$3"
        run_ADFFilter "$input_image" "$iteration" "$time_step" "$conductance"
    done
done
