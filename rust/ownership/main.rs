use std::thread;

struct xy 
{
    x: u64,
    y: u64,
}

struct task<'lt>
{
    point: &'lt xy,
}

fn execute(t: task)
{
    println!("current location: ({}, {})", t.point.x, t.point.x);
}

fn main()
{
    let t: Box<task>;
    {
        let p = Box::new(xy {x: 0, y: 0});
        let s = task {point: &p};

        {

            let f = ||
            {
                execute(s);
            };
            let t = thread::spawn(f);
            t.join();
        }
    }
}
