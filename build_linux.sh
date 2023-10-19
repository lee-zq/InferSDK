build_dir="build"

if [ -d "${build_dir}" ]; then
    rm -rf ${build_dir}/*
fi


# 检查依赖组件
OpenCV_DIR=/opt/envs/opencv/opencv-3.4.16/share/OpenCV
if [ ! -d "${OpenCV_DIR}" ]; then
    echo "OpenCV_DIR not found: ${OpenCV_DIR}"
    exit 1
fi
export OpenCV_DIR=${OpenCV_DIR}
# CUDA_ROOT="/usr/local/cuda-11.2"
# if [ ! -d "${CUDA_ROOT}" ]; then
#     echo "CUDA_ROOT not found: ${CUDA_ROOT}"
#     exit 1
# fi

cd ${build_dir}
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j2