# tian_server

# Start In Docker
docker run  -p 8822:22 -p 8888:8888  gzg1984/tian

# To See Port
docker port <  container ID as 4438d734edc0 >

#  Login
```
 ssh root@127.0.0.1 -p 8822
```

# Test
```
telnet 127.0.0.1 8888
# or
nc 127.0.0.1 8888
```
