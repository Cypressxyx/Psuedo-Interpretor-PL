						           							INDENTATION PSEUDO CODE
#Only counting spaces as tabs
#require a stack
#require a vector
#idx starts at 0
#Dont think I need a vector maybe just stack
[no indent]{expr}
[no indent]{for expr}
	[require indent (a)]{expr} #push indent to stack & vector if indent else err
	[indent]{expr} # continue if indent == vector[idx] else err
	[indent]{if expr} #idx += 1
		[require an indent(b)]{expr} #push indent to vec and stack if indent else err
		[indent]{expr} #continue if indent == vector[idx] else err
	[dedent] {expr} #check if indent is a dedent
									#Check by doing (vector[idx] - currIndent) % 8 == 0 ?
									#Pop off indent stack and check dedent = indent popped off
			


--------------                ------------
|    Stack   |								|  vector  |
--------------								------------
indent_a = 8 									v[0] = 8
indent_b = 8*2								v[1] = 8 * 2
	


