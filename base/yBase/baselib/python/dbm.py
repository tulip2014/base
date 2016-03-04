
import dbm

print(dir(dbm.dbm))

db = dbm.open("a", "c")

#db['www'] = "python";

db.close();
