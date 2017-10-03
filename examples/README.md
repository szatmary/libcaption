The examples folder contains sample code on how to use libcaption. The examples
also make for useful tools. Many of the tools operate on flv files containing
h264 video stream. Traditionally flv has not used 608/708 captions. However due
flv's simplicity, it is use within the libcaption tools to avoid external
dependencies.

# Codec support
libcaption itself is codec agnostic, The tools in this directory are designed to
work exclusively on h.264 (avc) encoded elementary streams. The tools capable of
reading/writing flv file will pass through any non AVC tags unmodified

# Tools
All tools accept flv as either a file path, or via stdio. use `-` to specify stdin/stdout
- `flv2srt` - reads a 608/708 captioned flv file and writes an srt file.
- `flv+srt` - encodes an srt file to 608/708 into pop-on captions and combines it with an existing flv, resulting in a new file
- `flv+scc` - same as flv+srt, but accepts an scc as input. However pin on/rool up is available as it is already encoded in the scc.
- `scc2srt` - converts an scc file to a srt file
- `ts2srt` - same as flv2srt, but reads from a mpeg2 transport stream

- add_captions - A shell script that utilizes ffmpeg to remux files to/from flv to simplify captioning
non flv file. This tool is not installed by `make install`, and serves primarily as an example.
Copy it and modify it to fit your needs.

# Usage
- Adding captions to an flv file:

`flv+srt in.flv in.srt out.flv`

- Alternatively,

`flv+srt in.flv in.srt - >out.flv`

- To caption a non-flv file

`ffmpeg -i in.mp4 -codec copy -f flv - | flv+srt - in.srt - | ffmpeg -i - -codec copy out.mp4`

# Advanced Usage
flv+srt is capable of reading an srt from a named pipe for the use in live captioning.
The input must still be formatted as an srt. If a null character is received on the pipe
The internal srt buffer is cleared. In this case the first srt timestamps after the null
is assumed to be immediate, and captions begins with this text on the next frame.
