idf_component_register(
    SRCS "sdstorage.c"
    INCLUDE_DIRS "include"
    REQUIRES driver fatfs sdmmc vfs
)