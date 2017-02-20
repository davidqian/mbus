source /opt/rh/devtoolset-2/enable
rm -rf ./build
node-gyp configure
node-gyp rebuild
