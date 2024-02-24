
# Provide your paths to the protoc and protoc_gen_grpc_path components
protoc="/Users/Vladislav.Artiukhov/.local/bin/protoc" # or: 'protoc' if it is set in PATH
protoc_gen_grpc_path="/Users/Vladislav.Artiukhov/.local/bin/grpc_cpp_plugin" # or: `which protoc_gen_grpc_path` if protoc_gen_grpc_path is set in PATH

# project path
project_root="$(pwd)"
# Where to store the generated sources (if changed, adjust included directories of the targets in the root CMakeLists.txt)
dest_dir="${project_root}/generated"
# Path to folder with proto-files
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
       --plugin=protoc-gen-grpc="${protoc_gen_grpc_path}" \
       "${proto_files_dir}/"*.proto
