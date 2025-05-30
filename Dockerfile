FROM randomdude/gcc-cross-x86_64-elf

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y \
    qemu-system \
    grub-pc-bin \
    grub-common \
    xorriso \
    nasm \
    make \
    mtools

RUN ln -s /usr/bin/grub-mkrescue /usr/bin/grub2-mkrescue

COPY . .
VOLUME /os
WORKDIR /os

CMD ["make"]


