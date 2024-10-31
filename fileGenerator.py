import random

file_size = 1000 * 1024 * 1024 # 2GB
alphabet = 'abcdefghijklmnopqrstuvwxyz '
file_path = 'test1000.txt'

with open(file_path, 'w') as f:
    f.write(''.join(random.choices(alphabet, k=file_size)))