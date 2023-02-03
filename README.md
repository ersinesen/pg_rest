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
```


## Usage

In psql:
```
select rest_get('https://ersinesen.appspot.com');
```