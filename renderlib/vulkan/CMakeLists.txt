target_include_directories(renderlib PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	${Boost_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${TIFF_INCLUDE_DIR}
	${GLAD_DIR}
	${Vulkan_INCLUDE_DIRS}
)
target_sources(renderlib PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/vulkanapp.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/vulkanapp.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/vulkandevice.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/vulkandevice.h"
)
