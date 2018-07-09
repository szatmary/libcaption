set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/")
# To find and use ffmpeg avcodec library
find_package( FFMPEG  )

add_executable(dumpcea608 dumpcea608.c)
include_directories(${FFMPEG_INCLUDE_DIRS})
target_link_libraries(dumpcea608 caption ${FFMPEG_LIBRARIES})
install(TARGETS dumpcea608 DESTINATION bin)


add_executable(flv2srt flv2srt.c flv.c)
target_link_libraries(flv2srt caption)
install(TARGETS flv2srt DESTINATION bin)

add_executable(ts2srt ts2srt.c ts.c)
target_link_libraries(ts2srt caption)
install(TARGETS ts2srt DESTINATION bin)

add_executable(srt2vtt srt2vtt.c)
target_link_libraries(srt2vtt caption)
install(TARGETS srt2vtt DESTINATION bin)

add_executable(scc2srt scc2srt.c)
target_link_libraries(scc2srt caption)
install(TARGETS scc2srt DESTINATION bin)

add_executable(flv+srt flv+srt.c flv.c)
target_link_libraries(flv+srt caption)
install(TARGETS flv+srt DESTINATION bin)

add_executable(flv+scc flv+scc.c flv.c)
target_link_libraries(flv+scc caption)
install(TARGETS flv+scc DESTINATION bin)

add_executable(sccdump sccdump.c flv.c)
target_link_libraries(sccdump caption)
install(TARGETS sccdump DESTINATION bin)

add_executable(srtdump srtdump.c)
target_link_libraries(srtdump caption)
install(TARGETS srtdump DESTINATION bin)

add_executable(vttdump vttdump.c)
target_link_libraries(vttdump caption)
install(TARGETS vttdump DESTINATION bin)

add_executable(party party.c flv.c)
target_link_libraries(party caption)
install(TARGETS party DESTINATION bin)

add_executable(vttsegmenter vttsegmenter.c)
target_link_libraries(vttsegmenter caption)
install(TARGETS vttsegmenter DESTINATION bin)

#add_executable(rtmpspit rtmpspit.c  flv.c)
#target_link_libraries(rtmpspit caption rtmp)
#install(TARGETS rtmpspit DESTINATION bin)
