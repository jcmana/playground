fn neco() -> i32
{
	println!("asdfsadfasdf");
	let b = false;
	if b
	{
		10
	}
	else
	{
		return 5;
	}
}

fn necojineho()
{
	if true
	{
		return;
	}

	println!("asdfsadrerwrcvsdf weat erg dsfzg");
}


fn main()
{
    //println!("Hello, world!");
    
    //let prom = neco();
    
    //println!("{}", prom);
    
    //necojineho();
    
    let clos = |a, b| return a == b;
    
    println!("asdfasdf {}", clos(1, 1));
}