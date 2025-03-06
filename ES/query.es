GET /books/_doc/1

GET books/_count

GET books/_search

GET books/_search
{
    "query": {
        "ids": {
            "values": [1]
        }
    },
    "_source": false
}

GET books/_search
{
    "query": {
        "match": {
          "author": "Joshua Doe"
        }
    }
}

GET books/_search
{
    "query": {
        "prefix": {
          "author": "josh"
        }
    }
}

GET books/_search
{
    "query": {
        "match": {
          "title": {
            "query": "effective java",
            "operator": "and"
          }
        }
    }
}



POST _bulk
{"index":{"_index":"books","_id":"1"}}
{"title": "Core Java Volume I – Fundamentals","author": "Cay S. Horstmann","edition": 11, "synopsis": "Java reference book that offers a detailed explanation of various features of Core Java, including exception handling, interfaces, and lambda expressions. Significant highlights of the book include simple language, conciseness, and detailed examples.","amazon_rating": 4.6,"release_date": "2018-08-27","tags": ["Programming Languages, Java Programming"]}
{"index":{"_index":"books","_id":"2"}}
{"title": "Effective Java","author": "Joshua Bloch", "edition": 3,"synopsis": "A must-have book for every Java programmer and Java aspirant, Effective Java makes up for an excellent complementary read with other Java books or learning material. The book offers 78 best practices to follow for making the code better.", "amazon_rating": 4.7, "release_date": "2017-12-27", "tags": ["Object Oriented Software Design"]}
{"index":{"_index":"books","_id":"3"}}
{"title": "Java: A Beginner’s Guide", "author": "Herbert Schildt","edition": 8,"synopsis": "One of the most comprehensive books for learning Java. The book offers several hands-on exercises as well as a quiz section at the end of every chapter to let the readers self-evaluate their learning.","amazon_rating": 4.2,"release_date": "2018-11-20","tags": ["Software Design & Engineering", "Internet & Web"]}
{"index":{"_index":"books","_id":"4"}}
{"title": "Java - The Complete Reference","author": "Herbert Schildt","edition": 11,"synopsis": "Convenient Java reference book examining essential portions of the Java API library, Java. The book is full of discussions and apt examples to better Java learning.","amazon_rating": 4.4,"release_date": "2019-03-19","tags": ["Software Design & Engineering", "Internet & Web", "Computer Programming Language & Tool"]}
{"index":{"_index":"books","_id":"5"}}
{"title": "Head First Java","author": "Kathy Sierra and Bert Bates","edition":2, "synopsis": "The most important selling points of Head First Java is its simplicity and super-effective real-life analogies that pertain to the Java programming concepts.","amazon_rating": 4.3,"release_date": "2005-02-18","tags": ["IT Certification Exams", "Object-Oriented Software Design","Design Pattern Programming"]}
{"index":{"_index":"books","_id":"6"}}
{"title": "Java Concurrency in Practice","author": "Brian Goetz with Tim Peierls, Joshua Bloch, Joseph Bowbeer, David Holmes, and Doug Lea","edition": 1,"synopsis": "Java Concurrency in Practice is one of the best Java programming books to develop a rich understanding of concurrency and multithreading.","amazon_rating": 4.3,"release_date": "2006-05-09","tags": ["Computer Science Books", "Programming Languages", "Java Programming"]}
{"index":{"_index":"books","_id":"7"}}
{"title": "Test-Driven: TDD and Acceptance TDD for Java Developers","author": "Lasse Koskela","edition": 1,"synopsis": "Test-Driven is an excellent book for learning how to write unique automation testing programs. It is a must-have book for those Java developers that prioritize code quality as well as have a knack for writing unit, integration, and automation tests.","amazon_rating": 4.1,"release_date": "2007-10-22","tags": ["Software Architecture", "Software Design & Engineering", "Java Programming"]}
{"index":{"_index":"books","_id":"8"}}
{"title": "Head First Object-Oriented Analysis Design","author": "Brett D. McLaughlin, Gary Pollice & David West","edition": 1,"synopsis": "Head First is one of the most beautiful finest book series ever written on Java programming language. Another gem in the series is the Head First Object-Oriented Analysis Design.","amazon_rating": 3.9,"release_date": "2014-04-29","tags": ["Introductory & Beginning Programming", "Object-Oriented Software Design", "Java Programming"]}
{"index":{"_index":"books","_id":"9"}}
{"title": "Java Performance: The Definite Guide","author": "Scott Oaks","edition": 1,"synopsis": "Garbage collection, JVM, and performance tuning are some of the most favorable aspects of the Java programming language. It educates readers about maximizing Java threading and synchronization performance features, improve Java-driven database application performance, tackle performance issues","amazon_rating": 4.1,"release_date": "2014-03-04","tags": ["Design Pattern Programming", "Object-Oriented Software Design", "Computer Programming Language & Tool"]}
{"index":{"_index":"books","_id":"10"}}
{"title": "Head First Design Patterns", "author": "Eric Freeman & Elisabeth Robson with Kathy Sierra & Bert Bates","edition": 10,"synopsis": "Head First Design Patterns is one of the leading books to build that particular understanding of the Java programming language." ,"amazon_rating": 4.5,"release_date": "2014-03-04","tags": ["Design Pattern Programming", "Object-Oriented Software Design eTextbooks", "Web Development & Design eTextbooks"]}




GET books/_search
{
    "query": {
        "multi_match": {
          "query": "Java effctive concurrency",
          "fields": ["title^2", "synopis"]
        }
    }
}

GET books/_search
{
  "query": {
    "match_phrase": {
      "synopsis": {
        "query": "must have book",
        "slop": 1
      }
    }
  },
  "highlight": {
    "fields": {
      "synopsis": {}
    }
  }
}

PUT books/_doc/99
{
"title":"Java Collections Deep Dive"
}
PUT books/_doc/100
{
"title":"Java Computing World"
}


GET books/_search
{
    "query": {
        "match_phrase_prefix": {
          "title": "java co"
        }
    }
}

## fuzzy
GET books/_search
{
    "query": {
        "fuzzy": {
          "title": {
            "value": "kava",
            "fuzziness": 1
          }
        }
    }
}

# term query
GET books/_search
{
    "_source": ["title", "edition"],
    "query": {
        "term": {
          "edition": {
            "value": 3
          }
        }
    }
}

# range query
GET books/_search
{
    "query": {
        "range": {
          "amazon_rating": {
            "gte": 4.5,
            "lte": 5
          }
        }
    }
}



