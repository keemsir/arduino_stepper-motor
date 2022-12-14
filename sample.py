def solution(arr):
    answer = []
    if len(range(arr)) > 1:
        answer = arr.pop(min(arr))
    else:
        answer = [-1]

    return answer


solution([4,3,2,1])