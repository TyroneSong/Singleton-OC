# Singleton-OC


[文章链接](https://juejin.im/post/5cb067676fb9a0688360f987?utm_source=gold_browser_extension)

### V1.0
```objc
@implementation SingletonClass
+ (instancetype)sharedInstance {
    static SingletonClass *instance = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        instance = [[self alloc] init];
    });
    return instance;
}
//...
@end

```


### V2.0
```language
static SingletonClass *sharedInstance = nil;

@implementation SingletonClass
#pragma mark Singleton Methods
+ (id)sharedInstance {
  @synchronized(self) {
      if(sharedInstance == nil)
          sharedInstance = [[super allocWithZone:NULL] init];
  }
  return sharedInstance;
}
+ (id)allocWithZone:(NSZone *)zone {
  return [[self sharedInstance] retain];
}
- (id)copyWithZone:(NSZone *)zone {
  return self;
}
- (id)retain {
  return self;
}
- (unsigned)retainCount {
  return UINT_MAX; //denotes an object that cannot be released
}
- (oneway void)release {
  // never release
}
- (id)autorelease {
  return self;
}
- (void)dealloc {
  // Should never be called, but just here for clarity really.
  [someProperty release];
  [super dealloc];
}
@end

```

### V3.0
```language

```

### V4.0
```language

```
### V5.0
```language

static SingletonClass *instance_SingletonClass = nil;

+ (id)sharedInstance {
    __block SingletonClass *strongRef = instance_SingletonClass;
    if (strongRef == nil) {
        static dispatch_semaphore_t lock;
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            lock = dispatch_semaphore_create(1);
        });
        dispatch_semaphore_wait(lock, DISPATCH_TIME_FOREVER);
        if (instance_SingletonClass == nil) {
            strongRef = [[super allocWithZone:NULL] init];
            instance_SingletonClass = strongRef;
        } else {
            strongRef = instance_SingletonClass;
        }
        dispatch_semaphore_signal(lock);
    }
    return strongRef;
}
```