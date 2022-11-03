APPLICATION_NAME = f1telem

all:
	@$(MAKE) deps test success || $(MAKE) failure

deps: 
	go mod download
	go mod verify

clean:
	go clean
	if [ -f ./coverage.html ]; then rm ./coverage.html; fi;
	if [ -f ./coverage.out ]; then rm ./coverage.out; fi;

test:
	go test ./... -coverprofile=coverage.out -bench . -count=1
	go tool cover -html=coverage.out -o coverage.html

success:
	printf "\n\e[1;32mTests Successful\e[0m\n"

failure:
	printf "\n\e[1;31mTest Failure\e[0m\n"
	exit 1

docker-build:
	docker-compose build