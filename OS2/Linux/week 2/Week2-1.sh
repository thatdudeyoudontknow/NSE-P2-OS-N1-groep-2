#!/bin/bash

# Directory containing the images
images_dir="/home/abe/Original"

# Function to move images to corresponding directories and compare hashes
organize_images() {
    local image_dir=$1

    # Loop through all image files in the directory
    for image in "$image_dir"/*; do
        if [ -f "$image" ]; then
            # Get the creation date of the image file
            creation_date=$(date -r "$image" '+%Y-%m-%d %H:%M:%S')

            # Extract year, month, and week from the creation date
            year=$(date -d "$creation_date" '+%Y')
            month=$(date -d "$creation_date" '+%m')
            week=$(date -d "$creation_date" '+%U')

            # Create directories for year, month, and week if they don't exist
            mkdir -p "$images_dir/$year/$month/Week_$week"

            # Obtain the original and moved file paths
            original_path="$image"
            moved_path="$images_dir/$year/$month/Week_$week/$(basename "$image")"

            # Make an exact copy of the image
            cp "$original_path" "$moved_path"

            original=$(md5sum "$original_path" | awk '{print $1}')
            moved=$(md5sum "$moved_path" | awk '{print $1}')

            # Print the hashes
            echo "Original hash: $original"
            echo "Moved hash: $moved"

            # Remove the original image only if the hash is equal
            if [ "$original" == "$moved" ]; then
                rm "$original_path"
            fi

            echo "Image processed: $moved_path"
        fi
    done
}

# Call the function to organize images
organize_images "$images_dir"
