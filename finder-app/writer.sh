
#!/bin/bash

if [ $# -lt 2 ]; then
        echo "Error: Argument missing!"
        exit 1
fi


for file_path_info in "$1"; do
	mkdir -p -- "$(dirname -- "$file_path_info")"
	touch -- "$file_path_info"
done

echo "$2" >> "$1"
