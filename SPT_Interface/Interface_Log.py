import logging

class SPTLog:
    def __init__(self, filename="SPT_Server.log"):
        self.logger = logging.getLogger(self.__class__.__name__)
        filehandler = logging.FileHandler(filename)
        formatter = logging.Formatter('%(asctime)s %(levelname)s: %(message)s |')
        filehandler.setFormatter(formatter)
        self.logger.addHandler(filehandler)
        self.logger.setLevel("INFO")

def info_log(self, text):
    self.logger.info(f"{self} - {text}")

def error_log(self, text):
    self.logger.error(f"{self} - {text}")

def log_function(func):
    def wrapper(self, *args, **kwargs):
        info_log(self, f"{func.__name__}: args {args}, kwargs {kwargs}")
        return func(self, *args, **kwargs)
    return wrapper

def log_function_response(func):
    def wrapper(self, *args, **kwargs):
        info_log(self, f"{func.__name__}: args {args}, kwargs {kwargs}")
        return_value = func(self, *args, **kwargs)
        info_log(self, f"{func.__name__}: complete")
        return return_value
    return wrapper
