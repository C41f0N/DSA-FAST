data = [12, 5, 23, 5, 6, 6, 6, 4, 78, 43, 99, 90, 9, 5, 3, 42, 44]


smallest = 0
for i in range(0, len(data)):
    for j in range(i, len(data)):
        if data[j] < data[smallest]:
            smallest = j

    for j in range(smallest, i - 1, -1):
        temp = data[j]
        data[j] = data[j - 1]
        data[j - 1] = temp

    print(data)


print(data)
