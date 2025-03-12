GET books/_search
{
  "query": {
    "bool": {
      "must": [
        {
          "match": {
            "author": "Joshua"
          }
        },
        {
          "range": {
            "amazon_rating": {
              "gte": 4.7
            }
          }
        }
      ],
      "must_not": [
        {
          "range": {
            "amazon_rating": {
              "lt": 4.7
            }
          }
        }
      ],
      "should": [
        {
          "match": {
            "tags": "software"
          }
        }
      ],
      "filter": [
        {
          "range": {
            "release_date": {
              "gt": "2015-01-01"
            }
          }
        },
        {
          "term": {
            "edition": 3
          }
        }
      ]
    }
  }
}