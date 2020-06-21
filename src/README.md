# TODO
- ~~[ ] Factory reset (reset credentials on long button press)~~
- [ ] Don't start HTTP client request too early
- [ ] HTTP server doen't response
- [ ] Don't send HTTP client request if previous request didn't receive a response
- ~~[ ] Make common code as a library~~
- ~~[ ] Update Dockerfile / docker-compose~~

# HOW TO

You should:

```bash
cp  common.mk.template common.mk                               # common.nk will be used to build each node
export SMING_HOME=/home/Path/To/Sming/Home                     # The project requires to know where Sming is to be built
export PROJECT_PATH=/home/path/to/this/git/repository/node_fw  # The project requires to know the path to common.mk to be built
```

- Update component.mk for each node
- Complete python server (server.py)
