const http= require('http');
const port =3000;

const server=http.createServer(function(req,res){
res.write('Hissdd');
res.end();
});

server.listen(port, function(error) {
    if (error){
        console.log('there is an error', error);
    } else{
        console.log('server is listening on port', port);
     }

})

