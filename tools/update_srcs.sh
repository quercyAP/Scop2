#!/bin/sh

current_dir=$1

srcs=$(find "$current_dir" -type f -name '*.cpp' -o -name '*.tpp' -o -name '*.ipp' ! -path "$current_dir/library/*" | sed "s|^$current_dir/src/||" | awk '{printf(" %s \\\\\n\t\t\t\t", $0)}')
hdr_flags=$(find "$current_dir/includes" -type d | sed "s|^$current_dir/| -I |" | awk '{printf("%s \\\\\n\t\t\t\t", $0)}')

perl -0777 -i -pe "BEGIN {undef $/; \$srcs = qq(${srcs}); \$hdr_flags = qq(${hdr_flags})} s/(SRCS\s+=).*?(?=SRC_DIR\s+=)/\$1\$srcs\\n/smg; s/(HDR_FLAGS\s+=).*?(?=LIB\s+)/\$1\$hdr_flags\\n/smg" "$current_dir/Makefile"