extends Node2D

func benchmark() -> int:
	var sum = 0
	for i in range(0, 10000000):
		sum = sum + 1
	return sum

func _ready():
	var start_time = Time.get_ticks_msec()
	benchmark()
	var end_time = Time.get_ticks_msec()
	var result = end_time - start_time 
	print("gdscript time to run in ms:")
	print(result)
