type = false

tab = {
	name = "hello world",
	age = 18,
	sex = "boy",
	des = "is a teacher",
	fun = function (name)
		print("hello " .. name)
		return 1,2,3,4,5
	end
}

function hello0()
	print("hello is no param")
end

function hello2(name)
	print("hello ", name)
end

function hello3(str1, str2)
	print(str1, str2)
	return 1,"lua"
end

