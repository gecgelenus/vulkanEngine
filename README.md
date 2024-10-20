Small repo that I use while learning vulkan. It is not intented to be full fledged engine, rather its a sketch book for learning things.

Update after about a month and week; Its going to be something useful! But I am too busy(or lazy) to fully documenting what I have done.
Instead I will summarize what it is capable of, and document it on my free time. But I want to remind that even simple task are requiring 
deep knowledge and research, so I will count them as feature as well :^)

# What it is capable of
- Loading .obj files with their material data( .mtl files)
- Runtime object adding/removing
- FPS like controls
- Memory management done with Vulkan Memory allocator (Although I also implemented my own allocator functions in the past)
- Per object data tracking and management of Texture Data, Transform Data and Material Data
- Multiple lighting support (Blinn–Phong) 
- Texture indexing (So we can exceed texture slots)
- GUI imlemented with ImGUI
- Vertex data deduplication (I am working on optimized file structure so I can also reduce load times)
- Support for 2D and 3D rendering

# What it is NOT capable of;
- Ray tracing (Don't worry, I will. One day...)
- Shadow mapping
- Video playing feature
- Physics engine
- Things I couldn't imagine right now

I am working to implement things what it is not capable of, but priority goes to TODO list, you can check on root folder.
Also some parts of codes might obsolete, don't take it serious. It is constantly changing.  
