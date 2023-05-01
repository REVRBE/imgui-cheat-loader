<h1>imgui cheat loader base + XenForo authentication</h1>

![mygif](https://user-images.githubusercontent.com/129604052/235542905-78255494-d7f3-43f7-9c68-5fa298ca6646.gif)

<h2>Usage</h2>
https://github.com/noteffex/ImGui-Loader-Base

Basicly this repo, but I've added XenForo authentication which check the user/pass from server-side and returns rank on succesful login e.g. Admin/VIP/other rank name from XenForo. This essentially is made for authenticating subscriptions by checking the rank of the forum-user. If the correct rank is returned, then access to load cheat is given.

It works with default installation of XenForo.

1. You need to put "login.php" file in your web-hotel (put it in standard XenForo folder) and update the database settings. 
2. Update a few lines of code and that should be all.

<h3>TO-DO on this project:</h3>
  ✅ Added XenForo support. (username, password & highest rank)
  
  ✅ Added nice colorways 
  
  ❌ Add "days left" to rank VIP to track subscriptions. (or other rank)
  
  ❌ Add encryption for data transmissions (client to server) - SHA1 encryption on hashed passwords by default for XenForo
  
  ❌ Add more customizations to the UI

<h3>Personal Note</h3>

I don't know why, but instead of going to sleep I produced this.... :P
