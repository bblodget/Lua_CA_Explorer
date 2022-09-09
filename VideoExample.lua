
--PlayerTitle = "Squire"
--PlayerName = "Ciaran"
--PlayerFamily = "Wirral"
--PlayerLevel = 20

players = {}

players[0] = {Title = "Squaire", Name ="Ciaran", Family = "Wirral" , Level = 20}
players[1] = {Title = "Lord", Name ="Diego", Family = "Brazil" , Level = 50}

print("[LUA]  Hello World")

function AddStuff(a, b)
	print("[LUA] Addstuff("..a..", "..b..") called \n")
	return a * b
end

function GetPlayer(n)
	return players[n]
end

function DoAThing(a, b)
	print("[LUA] DoAThing("..a..", "..b..") called \n")

	c = HostFunction(a + 10, b*3)
	return c
end
