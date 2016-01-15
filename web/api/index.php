<?php

ini_set('display_errors', 'on');
use \Psr\Http\Message\ServerRequestInterface as Request;
use \Psr\Http\Message\ResponseInterface as Response;

require 'vendor/autoload.php';

require 'db.php';

$app = new \Slim\App;

$app->get('/scenes', function (Request $request, Response $response) {
    //$name = $request->getAttribute('name');
    $conn = mysqlConnect();
    $sth = $conn->query("SELECT id, nickname, title, description from scenes");
    $rows = array();
    while($r = mysqli_fetch_assoc($sth)) {
        $rows[] = $r;
    }
   $response->getBody()->write(json_encode($rows));
    return $response;
});

$app->get('/scene/{id}', function (Request $request, Response $response) {
    $id = $request->getAttribute('id');
    $conn = mysqlConnect();
    $sth = $conn->query("SELECT id, nickname, title, description, jsondata from scenes where id = $id");
    $rows = array();
    while($r = mysqli_fetch_assoc($sth)) {
        $rows[] = $r;
    }
   $response->getBody()->write(json_encode($rows));
   return $response;
});

/*insert into scenes (nickname, title, description, ipaddress, jsondata, lastaccess, added)
values('bog','a scene 2','how it works','129.4.2.5','{data:13}', NOW(), NOW())*/

$app->post('/scene', function (Request $request, Response $response) {
    $jsondata = $request->getBody();
    $conn = mysqlConnect();
    $sth = $conn->query("SELECT id, nickname, title, description, jsondata from scenes where id = $id");
    $response->getBody()->write($jsondata);
    return $response;
});

$app->run();

//$conn->close();

?>
