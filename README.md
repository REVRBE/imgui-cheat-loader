<h1>imgui cheat loader base + XenForo authentication</h1>

![loader](https://user-images.githubusercontent.com/129604052/235508142-7f9b58fa-b658-49a6-814a-5723b1c752c1.PNG)

<h2>Usage</h2>
https://github.com/noteffex/ImGui-Loader-Base

Basicly this repo, but I've added XenForo authentication which check the user/pass from server-side and returns rank on succesful login e.g. Admin/VIP/other rank name from XenForo. This essentially is made for authenticating subscriptions by checking the rank of the forum-user. If the correct rank is returned, then access to load cheat is given.

It works with default installation of XenForo.

1. You need to put "login.php" file in your web-hotel (put it in standard XenForo folder) and update the database settings. 
2. Update a few lines of code and that should be all.

<h3>TO-DO on this project:</h3>
![accept](https://user-images.githubusercontent.com/129604052/235535613-7b41c3e3-2fe2-40aa-a529-3cc160c8d4d6.png) Add XenForo support. (username, password & highest rank)
![cross](https://user-images.githubusercontent.com/129604052/235535752-e4476752-ff7b-43ca-9d6d-a0d010e84f4d.png) Add "days left" to rank VIP to track subscriptions. (or other rank)
![cross](https://user-images.githubusercontent.com/129604052/235536228-ba2e616e-78e8-439c-8fb4-e26a8e3396bd.png) Add encryption for data transmissions (client to server) - SHA1 encryption on hashed passwords by default for XenForo

<h3>Personal Note</h3>

I don't know why, but instead of going to sleep I produced this.... :P
