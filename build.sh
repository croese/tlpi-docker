function tlpi {
	docker run -it --cap-add=SYS_PTRACE --security-opt seccomp=unconfined \
      -v $PWD/src:/tlpi/src \
      tlpi
}