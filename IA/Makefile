##
## EPITECH PROJECT, 2024
## B-YEP-400-PAR-4-1-zappy-alexis.mer
## File description:
## Makefile
##

NAME = zappy_ai

all: $(NAME)

PWD = $(shell pwd)
VENV = .venv
PYTHONPATH ?= $(VENV)/lib/python3.11/site-packages
PACKAGE_NAME = ZappyIAPackage
PACKAGE_FOLDER = ./package/
PYTHON ?= $(VENV)/bin/python3.11
PIP ?= $(VENV)/bin/pip
REQUIREMENTS = requirements.txt
OS_NAME := $(shell uname -s)

ifeq ($(OS_NAME), Darwin)
    SED_INPLACE := sed -i ''
else
    SED_INPLACE := sed -i
endif

all: $(PACKAGE_NAME) $(NAME)

$(VENV):
	python3.11 -m venv $(VENV)
	$(PYTHON) -m ensurepip --upgrade
	$(PYTHON) -m pip install --upgrade pip

$(PACKAGE_NAME): $(VENV)
	$(PIP) install -r $(REQUIREMENTS)
	mkdir -p build
	cp -r $(PACKAGE_FOLDER) build/$(PACKAGE_NAME)
	$(PIP) install ./build/$(PACKAGE_NAME)
	cp -r build/$(PACKAGE_NAME) $(PYTHONPATH)/ZappyIAPackage

$(NAME): $(PACKAGE_NAME)
	cp main.py $(NAME)
	chmod 755 $(NAME)
	$(SED_INPLACE) "1s|.*|#!$(PWD)/$(VENV)/bin/python|" $(NAME)

clean:
	rm -rf build

fclean: clean
	rm -rf $(NAME)
	rm -rf $(VENV)

re: fclean all

tests_run: $(PACKAGE_NAME)
	$(PIP) install pytest
	cd package && ../$(PYTHON) -m pytest
	cd ..

.PHONY: re fclean all clean $(PACKAGE_NAME)
