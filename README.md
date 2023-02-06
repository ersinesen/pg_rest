# PostgreSQL REST API Interface

## Requirements


Install postgresql:

```
sudo apt install postgresql postgresql-contrib postgresql-server-dev
```

CLI access:

```
sudo -u postgres psql
sudo -u ersin psql
```

Create user and db:
```
sudo -u postgres createuser --interactive
createdb ersin
```

## Build

```
make pg_rest
```

## Install

```
sudo cp pg_rest.so /usr/lib/postgresql/14/lib/
```

In psql:

```
CREATE FUNCTION rest_get(text) RETURNS text AS 'pg_rest.so', 'rest_get' LANGUAGE C STRICT;
CREATE FUNCTION rest_post(text, text) RETURNS text AS 'pg_rest.so', 'rest_post' LANGUAGE C STRICT;
CREATE FUNCTION rest_call(text, text, text) RETURNS text AS 'pg_rest.so', 'rest_call' LANGUAGE C STRICT;
```


## Usage

In psql:
```
SELECT rest_get('https://ersinesen.appspot.com');
SELECT rest_post('https://httpbin.org/post', 'HELLO');
SELECT rest_call('DELETE', 'https://httpbin.org/delete', 'DELETE THIS');
SELECT rest_call('PATCH', 'https://httpbin.org/patch', 'THIS IS A PATCH');
SELECT rest_call('PUT', 'https://httpbin.org/put', 'PUT THIS');
```

Warning: Use ' in sql command not ".