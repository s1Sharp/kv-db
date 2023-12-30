
set <key> <value> (create or update value by key)
exists <key> (if exists return <true> as return type)
get <key> (if key exists return <value>, otherwise if key not exists return <NULL> as return type)
type <key> (return value type)
keys <pattern> limit <number> (return array of keys with limit by pattern match)
del <key> (delete value and key from database)