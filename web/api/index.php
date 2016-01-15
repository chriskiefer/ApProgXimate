<?php

ini_set('display_errors', 'on');
use \Psr\Http\Message\ServerRequestInterface as Request;
use \Psr\Http\Message\ResponseInterface as Response;

require 'vendor/autoload.php';

require 'db.php'; //this isn't in git as it contains confidential info, but it contains a function mysqlConnect() that returns a mysql connection

$app = new \Slim\App;

$app->get('/scenes', function (Request $request, Response $response) {
    //$name = $request->getAttribute('name');
    $conn = mysqlConnect();
    $sth = $conn->query("SELECT id, nickname as nickName, title, description as 'desc' from scenes");
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
    $sth = $conn->query("UPDATE scenes SET accessCount = accessCount + 1, lastAccess = NOW() where id = $id");
    $sth = $conn->query("SELECT id, nickname as nickName, title, description as 'desc', jsondata from scenes where id = $id");
    $rows = array();
    while($r = mysqli_fetch_assoc($sth)) {
        $rows[] = $r;
    }
   $response->getBody()->write(json_encode($rows));
   return $response;
});

$app->post('/scene', function (Request $request, Response $response) {
    $jsondata = $request->getBody();
    $obj = json_decode($jsondata);
    $conn = mysqlConnect();
    $nickName = $conn->real_escape_string($obj->nickName);
    $title = $conn->real_escape_string($obj->title);
    $descr = $conn->real_escape_string($obj->desc);
    $jsondata = $conn->real_escape_string($jsondata);
    $ip = $_SERVER['REMOTE_ADDR'];
    $sth = $conn->query("insert into scenes (nickname, title, description, ipaddress, jsondata, lastaccess, added) values('$nickName','$title','$descr','$ip','$jsondata', NOW(), NOW())");
    $response->getBody()->write($obj->nickName);
    return $response;
});

$app->run();

//$conn->close();

?>
