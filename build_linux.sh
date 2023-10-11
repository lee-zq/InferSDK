build_dir="build"

if [ -d "${build_dir}" ]; then
    rm -rf ${build_dir}
fi

# 检查依赖组件
OpenCV_DIR="/usr/local/opencv-4.5.1/lib/cmake/opencv4"
if [ ! -d "${OpenCV_DIR}" ]; then
    echo "OpenCV_DIR not found: ${OpenCV_DIR}"
    exit 1
fi
# CUDA_ROOT="/usr/local/cuda-11.2"
# if [ ! -d "${CUDA_ROOT}" ]; then
#     echo "CUDA_ROOT not found: ${CUDA_ROOT}"
#     exit 1
# fi

mkdir ${build_dir} && cd ${build_dir}
cmake .. -DCMAKE_BUILD_TYPE=Debug 
make -j8