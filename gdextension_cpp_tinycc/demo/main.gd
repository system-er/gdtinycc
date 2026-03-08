extends Node2D

func benchmark() -> int:
	var sum := 0
	var n1 := 0
	var n2 := 1
	for i in range(0, 10000000):
		var n := n2
		n2 = n2 + n1
		n1 = n
		sum += n2

	#print(sum)
	return sum

func _ready():
	var start_time = Time.get_ticks_msec()
	benchmark()
	var end_time = Time.get_ticks_msec()
	var result = end_time - start_time 
	print("gdscript time to run in ms:")
	print(result)
