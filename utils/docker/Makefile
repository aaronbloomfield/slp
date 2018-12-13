# get the proxy based on the hostname
ifeq ($(shell hostname),orion)
	PROXY=192.168.14.6
else
	PROXY=libra.cs.virginia.edu
endif

build:
	docker build --build-arg APT_PROXY=$(PROXY) -t slp .

build-no-cache:
	docker build --no-cache --build-arg APT_PROXY=$(PROXY) -t slp .

run:
	docker-compose up -d

doc:
	@echo nothing to do!

