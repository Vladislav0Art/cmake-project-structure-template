
protoc="/Users/Vladislav.Artiukhov/.local/bin/protoc"
protoc_gen_grpc="/Users/Vladislav.Artiukhov/.local/bin/grpc_cpp_plugin"

project_root="$(pwd)"
dest_dir="${project_root}/generated"
proto_files_dir="${project_root}/proto"

echo "Project root directory: ${project_root}"
echo "Build directory: ${dest_dir}"
echo "Protobuf files diretory: ${proto_files_dir}"
echo

echo "Creating destination directory if not present..."
mkdir -p "${dest_dir}"

echo "Generate source files from protobuf models..."
"${protoc}" --grpc_out "${dest_dir}" \
       --cpp_out "${dest_dir}" \
       -I "${proto_files_dir}" \
       --plugin=protoc-gen-grpc="${protoc_gen_grpc}" \
       "${proto_files_dir}/"*.proto

# Serves as a marker for CMake that the script has finished execution
touch "${dest_dir}/script_execution_complete.txt"