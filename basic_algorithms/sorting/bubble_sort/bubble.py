def bubble_sort(items):
    for i in range(len(items)):
        for j in range(len(items)- 1 -i):
            if(items[j] > items[j+1]):
                items[j], items[j+1] = items[j+1], items[j]
    return items



def main():
    items = [1,5,4,2,3,9,8]
    items_names = ['James', 'Adib', 'Fatemeh', 'David']
    sorted_items = bubble_sort(items)
    sorted_names = bubble_sort(items_names)
    print(sorted_items)
    print(sorted_names)

main()

