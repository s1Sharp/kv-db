gtest_proj = subproject('gtest')
gtest_dep = gtest_proj.get_variable('gtest_main_dep')

test_array = [
#	[test name -- test_exe_file -- test_src_file -- add_deps]
	['hello_test' , 'hello_exe' , 'hello.cpp' , [parser_dep, boost_deps]]
]

foreach test : test_array
	test(test.get(0),
		executable(
			test.get(1),
			test.get(2),
			dependencies
			: 
			[
				gtest_dep,
				test.get(3)
			]
	)
)
endforeach
